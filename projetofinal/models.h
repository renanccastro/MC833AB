#ifndef __MODELS_H_INCLUDED__   // if x.h hasn't been included yet...
#define __MODELS_H_INCLUDED__   //   #define this so the compiler knows it has been included

#include <list>
#include <jansson.h>
#include <vector>
#define COMMAND_JSON "command"
#define USER_JSON_NAME "user_name"
#define USER_JSON_STATUS "status"
#define USERS_JSON_ARRAY "users"
#define USER_JSON_LAST_CONNECTION "user_connection"
using namespace std;
enum Commands { SETUSERNAME, WHO, SEND, CREATEG, JOING, SENDG, EXIT, UNKNOWN };

typedef struct user{
	int fd;
	const char* name;
	bool online;
	char last_connection[50];
} user;

typedef struct message{
	int id;
	char* from;
	char* to;
	char date_sent[50];
} message;

typedef struct group{
	int groupId;
	std::list<char*> members;
} group;

void get_current_date_time(char *datestring, char *timestring);
char* get_online_users_json_string(vector<user> users);
Commands parse_json(char *json_string, const char *args[], json_error_t *error);
vector<const char*> parse_who_json_response(char* json_string, json_error_t *error);
char* set_username_json_string(char* name);
const char *get_who_json_string();
#endif 
