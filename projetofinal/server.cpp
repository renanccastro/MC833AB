#include "models.h"
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

#define LISTENQ 5
#define MAXLINE 4096
#define SERV_PORT 56789

using namespace std;
int main(int argc, char **argv) {
  int i, maxi, maxfd, listenfd, connfd, sockfd, j;
  vector<user> users;
  int nready, client[FD_SETSIZE];
  ssize_t n;
  fd_set rset, allset;
  char buf[MAXLINE];
  socklen_t clilen;
  struct sockaddr_in cliaddr, servaddr;
  char date_string[50], time_string[50];
  json_t *root;
  json_error_t error;

  if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket error");
    return 1;
  }

  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(SERV_PORT);

  if (bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
    perror("bind error");
    close(listenfd);
    return 1;
  }

  if (listen(listenfd, LISTENQ) < 0) {
    perror("listen error");
    close(listenfd);
    return 1;
  }

  maxfd = listenfd; /* initialize */
  maxi = -1;        /* index into client[] array */
  for (i = 0; i < FD_SETSIZE; i++)
    client[i] = -1; /* -1 indicates available entry */
  FD_ZERO(&allset);
  FD_SET(listenfd, &allset);

  for (;;) {
    rset = allset; /* structure assignment */
    nready = select(maxfd + 1, &rset, NULL, NULL, NULL);
    if (nready < 0) {
      perror("select error");
      return 1;
    }

    if (FD_ISSET(listenfd, &rset)) { /* new client connection */
      clilen = sizeof(cliaddr);
      connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen);
      if (connfd < 0) {
        perror("accept error");
        return 1;
      }
      user u = user();
      u.fd = connfd;
      u.online = true;
      get_current_date_time(date_string, time_string);
      sprintf(u.last_connection, "%s-%s", date_string, time_string);
      users.push_back(u);

      FD_SET(connfd, &allset); /* add new descriptor to set */
      if (connfd > maxfd)
        maxfd = connfd; /* for select */
      // if (i > maxi)
      // maxi = i; /* max index in client[] array */

      if (--nready <= 0)
        continue; /* no more readable descriptors */
    }

    for (i = 0; i <= users.size(); i++) { /* check all clients for data */
      if ((sockfd = users[i].fd) < 0)
        continue;
      if (FD_ISSET(sockfd, &rset)) {
        if ((n = read(sockfd, buf, MAXLINE)) == 0) {
          if (n < 0) {
            perror("read error");
            return 1;
          }
          /* connection closed by client */
          get_current_date_time(date_string, time_string);
          sprintf(users[i].last_connection, "%s-%s", date_string, time_string);
          close(sockfd);
          FD_CLR(sockfd, &allset);
          users[i].fd = -1;
          users[i].online = false;
        } else {
          // Recebeu algum pacote do cliente users[i]
          // Parseia
          const char *json_args[10];
		  char *json_rep;
		  buf[n]='\0';
		  printf("Message received: %s\n", buf);
          Commands c = parse_json(buf, json_args, &error);
          if (c == WHO) {
             json_rep = get_online_users_json_string(users);
            send(sockfd, json_rep, strlen(json_rep), 0);
          }else if(c == SETUSERNAME){
			printf("settings username: %s",json_args[0]);
          	users[i].name = json_args[0];
          }
          // if (send(sockfd, buf, n, 0) < 0) {
          //   perror("read error");
          //   return 1;
          // }
        }
        if (--nready <= 0)
          break; /* no more readable descriptors */
      }
    }
  }
}

