#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

#define LISTENQ 5
#define MAXLINE 64
#define SERV_PORT 56789

int main(int argc, char **argv) {
  int i, maxi, maxfd, listenfd, connfd, sockfd, recvlen;
  int nready, client[FD_SETSIZE];
  ssize_t n;
  fd_set rset, allset;
  char buf[MAXLINE];
  socklen_t clilen;
  struct sockaddr_in servaddr;
  struct sockaddr_in remoteaddress;     /* remote address */
  socklen_t remoteaddresslength = sizeof(remoteaddress);            /* length of addresses */

  if ((listenfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
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
  for (;;) {
    recvlen = recvfrom(listenfd, buf, MAXLINE, 0, (struct sockaddr *)&remoteaddress, &remoteaddresslength);
    if (recvlen > 0) {
        buf[recvlen] = 0;
        fprintf(stdout, "receiving message from - %s:%d\n", inet_ntoa(remoteaddress.sin_addr),
                ntohs(remoteaddress.sin_port));
        fputs(buf, stdout);
        sendto(listenfd, buf, strlen(buf), 0, (struct sockaddr *)&remoteaddress, remoteaddresslength);
    }


  }

}
