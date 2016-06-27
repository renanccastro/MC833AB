#include "models.h"
#include <arpa/inet.h>
#include <iostream>
#include <ncurses.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

/* color pair identifiers - up to 8 - see man COLOR_PAIR */
enum {
  DATETIME,
  NICK,
  SEPARATOR,
  MESG,
  WINP,
};
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define NICKLEN 12
#define MAX_LINE 4096
using namespace std;

WINDOW *create_newwin(int height, int width, int starty, int startx);
void destroy_win(WINDOW *local_win);
void draw_windows(WINDOW **two, WINDOW **one, WINDOW **three);
Commands interpret(char *string, int *argc, char **argv);
static void printline(const char *date, const char *time, const char *nick,
                      const char *mesg, WINDOW *wout);

void clearWindows(WINDOW *w) {
  wclear(w);
  box(w, 0, 0);
  wrefresh(w);
}
int main(int argc, char *argv[]) {
  FILE *fp;
  socklen_t infoLen = sizeof(struct sockaddr_in);
  int s, recvlen, l, maxfd, nready;
  struct hostent *hp;
  Commands command;
  struct sockaddr_in sin, info, remoteaddress;
  socklen_t remoteaddresslength = sizeof(sin); /* length of addresses */
  WINDOW *input, *output, *list;
  char *host, *port, *nick;
  char string[1000];
  char *args;
  int len;
  char buf[MAX_LINE];
  int argcount;
  char *user_name_json;
  const char *temp;
  char str_time[100];
  char str_date[100];
  fd_set rset, allset;

  if (argc == 4) {
    host = argv[1];
    port = argv[2];
    nick = argv[3];
  } else {
    fprintf(stderr, "usage: ./client HOST PORT USER_NAME\n");
    exit(1);
  }
  /* translate host name into peer’s IP address */
  hp = gethostbyname(host);
  if (!hp) {
    fprintf(stderr, "simplex-talk: unknown host: %s\n", host);
    exit(1);
  }

  /* build address data structure */
  bzero((char *)&sin, sizeof(sin));
  sin.sin_family = AF_INET;
  bcopy(hp->h_addr, (char *)&sin.sin_addr, hp->h_length);
  sin.sin_port = htons(atoi(port));
  /* active open */
  if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("simplex-talk: socket");
    exit(1);
  }
  if (connect(s, (struct sockaddr *)&sin, sizeof(sin)) < 0) {
    perror("simplex-talk: connect");
    close(s);
    exit(1);
  }

  initscr(); /* Start curses mode 		*/
  raw();

  draw_windows(&input, &output, &list);
  user_name_json = set_username_json_string(nick);
  send(s, user_name_json, strlen(user_name_json), 0);
  getsockname(s, (struct sockaddr *)&info, &infoLen);
  sprintf(string, "Conectado em: %s:%d", inet_ntoa(info.sin_addr),
          ntohs(info.sin_port));
  get_current_date_time(str_date, str_time);

  FD_ZERO(&allset);
  maxfd = MAX(fileno(stdin), s);
  FD_SET(fileno(stdin), &allset);
  FD_SET(s, &allset);
  clearWindows(input);
  wmove(output, 1, 0);
  printline(str_date, str_time, "CLIENT", string, output);
  wrefresh(output);
  wmove(input, 1, 1);
  for (;;) {
    rset = allset;
    nready = select(maxfd + 1, &rset, NULL, NULL, NULL);
    if (nready < 0) {
      perror("select error");
      return 1;
    }
    if (FD_ISSET(s, &rset)) {
      int read_lines;
      if ((read_lines = recv(s, buf, sizeof(buf), 0)) == 0) {
        // strange error
        perror("receive error");
        return 1;
      } else {
        buf[read_lines] = '\0';
        const char *json_args[10];
		char *json_rep;
	    json_error_t error;
        Commands c = parse_json(buf, json_args, &error);
        if (c == WHO) {
			int i;
		    get_current_date_time(str_date, str_time);
			vector<const char*> nicks = parse_who_json_response(buf,&error);
			for(i = 0; i<nicks.size(); i++){
			    printline(str_date, str_time, "SERVER", nicks[i], output);
			}
		}
      }
    }
    if (FD_ISSET(fileno(stdin), &rset)) {
      mvwgetstr(input, 1, 1, string);
      clearWindows(input);
      if ((command = interpret(string, &argcount, &args)) == EXIT)
        break;
      else {
        if (command == WHO) {
          temp = get_who_json_string();
          send(s, temp, strlen(temp), 0);
        }
      }
      wrefresh(input);
    }
  }

  endwin(); /* End curses mode		  */
  return 0;
}

static void printline(const char *date, const char *time, const char *nick,
                      const char *mesg, WINDOW *wout) {
  int x, y;
  getyx(wout, y, x);
  // if(y == LINES-4){
  // 	y = 1;
  // 	clearWindows(wout);
  // }
  wmove(wout, y, x + 1);
  if (has_colors() == TRUE)
    wattron(wout, COLOR_PAIR(SEPARATOR));
  wprintw(wout, "%s - %s ", date, time);
  if (has_colors() == TRUE)
    wattroff(wout, COLOR_PAIR(SEPARATOR));

  if (has_colors() == TRUE)
    wattron(wout, COLOR_PAIR(NICK));
  wprintw(wout, " %s ", nick);
  if (has_colors() == TRUE)
    wattroff(wout, COLOR_PAIR(NICK));

  if (has_colors() == TRUE)
    wattron(wout, COLOR_PAIR(SEPARATOR));
  wprintw(wout, "| ");
  if (has_colors() == TRUE)
    wattroff(wout, COLOR_PAIR(SEPARATOR));

  if (has_colors() == TRUE)
    wattron(wout, COLOR_PAIR(MESG));
  wprintw(wout, "%s\n", mesg);
  if (has_colors() == TRUE)
    wattroff(wout, COLOR_PAIR(MESG));
  box(wout, 0, 0);
  wrefresh(wout);
}

WINDOW *create_newwin(int height, int width, int starty, int startx) {
  WINDOW *local_win;

  local_win = newwin(height, width, starty, startx);
  box(local_win, 0,
      0);              /* 0, 0 gives default characters
                                                * for the vertical and horizontal
                                                * lines			*/
  wrefresh(local_win); /* Show that box 		*/

  return local_win;
}

void destroy_win(WINDOW *local_win) {
  wborder(local_win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
  wrefresh(local_win);
  delwin(local_win);
}

Commands interpret(char *string, int *argc, char **argv) {
  if (!strcmp(string, "EXIT")) {
    return EXIT;
  } else if (!strcmp(string, "WHO")) {
    return WHO;
  }
  return UNKNOWN;
}

void draw_windows(WINDOW **two, WINDOW **one, WINDOW **three) {
  *one = create_newwin(LINES - 3, COLS, 0, 0);
  scrollok(*one, TRUE);
  *two = create_newwin(3, 0, LINES - 3, 0);
  // *three = create_newwin(LINES-3,0,0,COLS-12);
  keypad(*two, TRUE); /* I need that nifty F1 	*/
  if (has_colors() == TRUE) {
    start_color();
    init_pair(DATETIME, COLOR_CYAN, COLOR_BLACK);
    init_pair(NICK, COLOR_GREEN, COLOR_BLACK);
    init_pair(SEPARATOR, COLOR_CYAN, COLOR_BLACK);
    init_pair(MESG, COLOR_WHITE, COLOR_BLACK);
    init_pair(WINP, COLOR_GREEN, COLOR_BLACK);
  }
  refresh();
}
