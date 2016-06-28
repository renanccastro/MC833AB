#include "models.h"

using namespace std;
void get_current_date_time(char *datestring, char *timestring) {
    time_t rawtime;
    struct tm * timeinfo;

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );

    sprintf(datestring, "[%d/%d]",timeinfo->tm_mday, timeinfo->tm_mon + 1);
	sprintf(timestring, "[%d:%d:%d]",timeinfo->tm_hour, timeinfo->tm_min,timeinfo->tm_sec);
	
}

Commands parse_json(char *json_string, const char *args[], json_error_t *error) {
  json_t *root, *c;
  json_int_t command;
  root = json_loads(json_string, 0, error);
  if(!root)
	  return UNKNOWN;
  c = json_object_get(root, COMMAND_JSON);
  command = json_integer_value(c);
  if(command == SETUSERNAME){
  	  args[0] = json_string_value(json_object_get(root, USER_JSON_NAME));
	  printf("received args[0]:%s\n",args[0]);
  }else if(command == SEND){
  	  args[0] = json_string_value(json_object_get(root, MESSAGE_JSON_TO));
	  args[1] = json_string_value(json_object_get(root, MESSAGE_JSON_MESSAGE));
  }else if(command == CREATEG || command == JOING){
  	  args[0] = json_string_value(json_object_get(root, GROUP_JSON_NAME));
  }else if(command == SENDG){
  	  args[0] = json_string_value(json_object_get(root, GROUP_JSON_NAME));
	  args[1] = json_string_value(json_object_get(root, MESSAGE_JSON_MESSAGE));
  }
  // json_decref(root);
  return (Commands)command;
}

vector<const char*> parse_who_json_response(char* json_string, json_error_t *error){
	vector<const char*> nicknames;
    json_t *root, *c;
	char* string;
	int i;
    root = json_loads(json_string, 0, error);
    c = json_object_get(root, USERS_JSON_ARRAY);
	for(i = 0; i < json_array_size(c); i++)
	{
		json_t *data;
	    const char *nick;
		const char *status;

	    data = json_array_get(c, i);

		nick = json_string_value(json_object_get(data, USER_JSON_NAME));
		status = json_string_value(json_object_get(data, USER_JSON_STATUS));
		
		string = (char*)malloc(strlen(nick)+3+strlen(status));
		sprintf(string, "%s - %s", nick,status);
		nicknames.push_back((const char*)string);
	}
    json_decref(root);
	
    return nicknames;
}

char* set_username_json_string(char* name){
    json_t *root = json_object();
    json_object_set_new(root, COMMAND_JSON, json_integer(SETUSERNAME));
    json_object_set_new(root, USER_JSON_NAME, json_string(name));
    char* r = json_dumps(root, 0);
    json_decref(root);  
    return r;
}
	
	
json_t *encode_user_json(user u) {
  json_t *root = json_object();
  printf("name: %s\n",u.name);
  json_object_set_new(root, USER_JSON_NAME, json_string(u.name));
  json_object_set_new(root, USER_JSON_STATUS, json_string(u.online == true ? "online" : "offline"));
  json_object_set_new(root, USER_JSON_LAST_CONNECTION,
                      json_string(u.last_connection));
					
					  
					  return root;
}

const char *get_who_json_string(){
	return "{\"command\":1}";
}

const char *send_message_string(const char* to, const char* message){
    json_t *root = json_object();
    json_object_set_new(root, COMMAND_JSON, json_integer(SEND));
	json_object_set_new(root, MESSAGE_JSON_TO, json_string(to));
    json_object_set_new(root, MESSAGE_JSON_MESSAGE, json_string(message));
    char* r = json_dumps(root, 0);
    json_decref(root);  
    return r;
}

const char *create_group_string(const char* string){
    json_t *root = json_object();
    json_object_set_new(root, COMMAND_JSON, json_integer(CREATEG));
    json_object_set_new(root, GROUP_JSON_NAME, json_string(string));
    char* r = json_dumps(root, 0);
    json_decref(root);  
    return r;
}

const char *join_group_string(const char* string){
    json_t *root = json_object();
    json_object_set_new(root, COMMAND_JSON, json_integer(JOING));
    json_object_set_new(root, GROUP_JSON_NAME, json_string(string));
    char* r = json_dumps(root, 0);
    json_decref(root);
    return r;
}
const char *send_group_string(const char* group, const char* message){
    json_t *root = json_object();
    json_object_set_new(root, COMMAND_JSON, json_integer(SENDG));
    json_object_set_new(root, GROUP_JSON_NAME, json_string(group));
	json_object_set_new(root, MESSAGE_JSON_MESSAGE, json_string(message));
    char* r = json_dumps(root, 0);
    json_decref(root);
    return r;
}


char *get_online_users_json_string(vector<user> users) {
  int j;
  json_t *root = json_object();
  json_object_set_new(root, COMMAND_JSON, json_integer(WHO));
  json_t *array = json_array();
  for (j = 0; j < users.size(); j++) {
		json_array_append( array, encode_user_json(users[j]));
  }
  json_object_set_new(root, USERS_JSON_ARRAY, array);
  
  char* r = json_dumps(root, 0);
  json_decref(root);
  
  return r;
}