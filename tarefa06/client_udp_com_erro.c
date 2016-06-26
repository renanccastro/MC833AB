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
	char *host;
	char buf[MAX_LINE];
	int s, recvlen, l;
	int len, shouldReadFromStdin = 1;
	if (argc==2) {
		host = argv[1];
	}
	else {
		fprintf(stderr, "usage: ./client host\n");
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

	/* active open */
	if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("simplex-talk: socket");
		exit(1);
	}

  // getsockname(s, (struct sockaddr *) &info, &infoLen);
  // fprintf(stdout, "listening on %s:%d\n", inet_ntoa(info.sin_addr),
  //         ntohs(info.sin_port));


  /* main loop: get and send lines of text */
  while (1) {

    fgets(buf, sizeof(buf), stdin);
    buf[MAX_LINE-1] = '\0';
    len = strlen(buf) + 1;
    l = sendto(s, buf, len, 0, (struct sockaddr *)&sin, remoteaddresslength);
		if(l < 0){
			printf("ERRO NO SEND");
		}

    recvlen = recvfrom(s, buf, MAX_LINE, 0, (struct sockaddr *)&remoteaddress, &remoteaddresslength);
		if(recvlen < 0){
			printf("ERRO NO RECEIVE");
		}

	  fprintf(stdout, "received message from - %s:%d\n", inet_ntoa(remoteaddress.sin_addr),
	          ntohs(remoteaddress.sin_port));
    if (recvlen >= 0) {
        buf[recvlen] = 0;
        printf("%s", buf);
    }
  }
  close(s);

}
