#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

#define LISTENQ 5
#define MAXLINE 64
#define SERV_PORT 56789
#define MAX(x, y) (((x) > (y)) ? (x) : (y))

int main(int argc, char **argv) {
  int listenfd, connfd, udpfd, nready, maxfd;
  char buf[MAXLINE];
  pid_t childpid;
  fd_set rset;
  ssize_t n;
  socklen_t len;
  const int on = 1;
  struct sockaddr_in cliaddr, servaddr;
  void sig_chld(int);

  /* create listening TCP socket */
  if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket error");
    return 1;
  }
  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(SERV_PORT);
  // seta a opção SO_REUSEADDR
  setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
  if (bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
    perror("bind error");
    close(listenfd);
    return 1;
  }
  listen(listenfd, LISTENQ);

  udpfd = socket(AF_INET, SOCK_DGRAM, 0);
  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(SERV_PORT);
  if (bind(udpfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
    perror("bind error");
    close(listenfd);
    return 1;
  }

  FD_ZERO(&rset);
  maxfd = MAX(listenfd, udpfd) + 1;
  for (;;) {
    FD_SET(listenfd, &rset);
    FD_SET(udpfd, &rset);
    nready = select(maxfd + 1, &rset, NULL, NULL, NULL);
		if (nready < 0) {
	    perror("select error");
	    return 1;
	  }

    if (FD_ISSET(listenfd, &rset)) {
      len = sizeof(cliaddr);
      connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &len);
      fprintf(stdout, "[TCP] client connected: - %s:%d\n", inet_ntoa(cliaddr.sin_addr),
              ntohs(cliaddr.sin_port));

      if ((fork()) == 0) { /* child process */
        close(listenfd);              /* close listening socket */
        while ((len = recv(connfd, buf, sizeof(buf), 0))){
          fprintf(stdout, "[TCP] message received: - %s:%d\n", inet_ntoa(cliaddr.sin_addr),
                  ntohs(cliaddr.sin_port));
  				// fputs(buf, stdout);
				  
  				send(connfd, buf, len, 0);
  			}
        exit(0);
      }
      close(connfd); /* parent closes connected socket */
    }

    if (FD_ISSET(udpfd, &rset)) {
      len = sizeof(cliaddr);
      n = recvfrom(udpfd, buf, MAXLINE, 0, (struct sockaddr *)&cliaddr, &len);
      fprintf(stdout, "[UDP] received message from - %s:%d\n", inet_ntoa(cliaddr.sin_addr),
              ntohs(cliaddr.sin_port));
      sendto(udpfd, buf, n, 0, (struct sockaddr *)&cliaddr, len);

    }
  }
  return 0;
}
