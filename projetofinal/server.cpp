#include "models.h"
#include <iostream>
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

void print_group_vector(vector<group> g) {
  for (int i = 0; i < g.size(); i++) {
    cout << "GRUPO: " << g[i].groupId << " - [";
    group a = g[i];
    list<user *>::const_iterator iterator;
    for (iterator = a.members.begin(); iterator != a.members.end();
         ++iterator) {
      cout << " " << (*iterator)->name;
    }
    cout << "]" << endl;
  }
}

int main(int argc, char **argv) {
  int i, maxi, maxfd, listenfd, connfd, sockfd, j;
  vector<user> users;
  vector<group> groups;
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

	bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
//    perror("bind error");
//    close(listenfd);
//    return 1;
//  }

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
      u.name = NULL;
      u.online = true;
      get_current_date_time(date_string, time_string);
      sprintf(u.last_connection, "%s-%s", date_string, time_string);
      users.push_back(u);
      printf("NEW USER ENTERED SERVER.\n");

      FD_SET(connfd, &allset); /* add new descriptor to set */
      if (connfd > maxfd)
        maxfd = connfd; /* for select */
      // if (i > maxi)
      // maxi = i; /* max index in client[] array */

      if (--nready <= 0)
        continue; /* no more readable descriptors */
    }

    vector<user>::iterator users_i;
    for (users_i = users.begin(); users_i != users.end(); ++users_i) {
      if (!(*users_i).q.empty() && (*users_i).online == true) {
        send((*users_i).fd, (*users_i).q.front(), strlen((*users_i).q.front()),
             0);
        (*users_i).q.pop();
      }
    }

    users_i = users.begin();
    while (users_i != users.end()) {
      bool shouldIncrement = true;
      if ((sockfd = (*users_i).fd) < 0){
				++users_i;
        continue;
			}
      if (FD_ISSET(sockfd, &rset)) {
        if ((n = read(sockfd, buf, MAXLINE)) == 0) {
          if (n < 0) {
            perror("read error");
            return 1;
          }
          /* connection closed by client */
          get_current_date_time(date_string, time_string);
          sprintf((*users_i).last_connection, "%s-%s", date_string,
                  time_string);
          close(sockfd);
          FD_CLR(sockfd, &allset);
          (*users_i).fd = -1;
          (*users_i).online = false;
        } else {
          // Recebeu algum pacote do cliente users[i]
          // Parseia
          const char *json_args[10];
          char *json_rep;
          buf[n] = '\0';
          printf("JSON RECEIVED: %s\n", buf);
          Commands c = parse_json(buf, json_args, &error);
          switch (c) {
          case WHO: {
            json_rep = get_online_users_json_string(users);
            send(sockfd, json_rep, strlen(json_rep), 0);
            break;
          }
          case SETUSERNAME: {
            vector<user>::iterator users_j;
            for (users_j = users.begin(); users_j != users.end(); ++users_j) {
              // se jah existe um usuario com nome
              if ((*users_j).name && !strcmp((*users_j).name, json_args[0])) {
                printf("%s LOGGED BACK.\n", json_args[0]);
                shouldIncrement = false;
                (*users_j).online = true;
                (*users_j).fd = (*users_i).fd;
                // apaga o usuario temporario
                users_i = users.erase(users_i);
                break;
              }
            }
            if (shouldIncrement) {
              (*users_i).name = json_args[0];
            }
            break;
          }
          case SEND: {
            for (int b = 0; b < users.size(); b++) {
              if (!strcmp(users[b].name, json_args[0])) {
                if (users[b].online == true) {
                  send(users[b].fd, json_args[1], strlen(json_args[1]), 0);
                } else {
                  users[b].q.push(json_args[1]);
                }
              }
            }
            break;
          }
          case CREATEG: {
            group g = group();
            g.groupId = json_args[0];
            groups.push_back(g);
            print_group_vector(groups);
            break;
          }
          case JOING: {
            for (int b = 0; b < groups.size(); b++) {
              if (!strcmp(groups[b].groupId, json_args[0])) {
                groups[b].members.push_back(&(*users_i));
              }
            }
            print_group_vector(groups);
            break;
          }
          case SENDG: {
            for (int b = 0; b < groups.size(); b++) {
              if (!strcmp(groups[b].groupId, json_args[0])) {
                group g = groups[b];
                list<user *>::const_iterator iterator;
                for (iterator = g.members.begin(); iterator != g.members.end();
                     ++iterator) {
                  if ((*iterator)->online == true) {
                    send((*iterator)->fd, json_args[1], strlen(json_args[1]),
                         0);
                  } else {
                    (*iterator)->q.push(json_args[1]);
                  }
                }
              }
            }
            break;
          }
          default: { break; }
          }
        }
        if (shouldIncrement) {
          users_i++;
        }
        if (--nready <= 0)
          break; /* no more readable descriptors */
      }
    }
  }
}
