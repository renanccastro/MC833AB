#include "models.h"

using namespace std;
void get_current_date_time(char *datestring, char *timestring) {
  struct tm *tm;
  time_t t;

  t = time(NULL);
  tm = localtime(&t);

  strftime(timestring, sizeof(timestring), "%H:%M:%S", tm);
  strftime(datestring, sizeof(datestring), "%d/%m", tm);
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
  }
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
    return nicknames;
}

char* set_username_json_string(char* name){
	char* a = (char*)malloc(sizeof(char)*255);
	sprintf(a, "{\"command\":%d,\"%s\":\"%s\"}", SETUSERNAME,USER_JSON_NAME,name);
	return a;
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