#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>


#define SERVER_PORT 56789
#define MAX_LINE 256

int main(int argc, char * argv[])
{
	FILE *fp;
	struct hostent *hp;
	struct sockaddr_in sin, info, remoteaddress;
  socklen_t remoteaddresslength = sizeof(sin);            /* length of addresses */

	socklen_t infoLen = sizeof(struct sockaddr_in);
	char *host,*socket_type;
	char buf[MAX_LINE];
	int s, recvlen, l;
	int len, shouldReadFromStdin = 1;
	if (argc==3) {
		host = argv[1];
    socket_type = argv[2];
	}
	else {
		 fprintf(stderr, "usage: ./client HOST SOCKET_TYPE\n");
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
  sin.sin_port = htons(SERVER_PORT);

  int socket_type_num = !strcmp("tcp",socket_type) ? SOCK_STREAM : SOCK_DGRAM;
  /* active open */
  if ((s = socket(AF_INET, socket_type_num, 0)) < 0) {
    perror("simplex-talk: socket");
    exit(1);
  }
  //se for udp, cliente é UDP conectado, se for TCP é conectado tbm
  if (connect(s, (struct sockaddr *)&sin, sizeof(sin)) < 0) {
		perror("simplex-talk: connect");
		close(s);
		exit(1);
	}

  getsockname(s, (struct sockaddr *) &info, &infoLen);
  fprintf(stdout, "listening on %s:%d\n", inet_ntoa(info.sin_addr),
          ntohs(info.sin_port));

  while (fgets(buf, sizeof(buf), stdin)) {
		buf[MAX_LINE-1] = '\0';
		len = strlen(buf) + 1;
		send(s, buf, len, 0);
		recv(s, buf, sizeof(buf), 0);
		fputs(buf, stdout);
	}
}
