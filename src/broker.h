#ifndef SAPO_BROKER_H
#define SAPO_BROKER_H

#include <jansson.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xpath.h>


typedef enum{
        PUBLISH = 0,
        POLL = 1,
        ACCEPTED = 2,
        ACKNOWLEDGE_MESSAGE = 3,
        SUBSCRIBE = 4,
        UNSUBSCRIBE = 5,
        NOTIFICATION = 6,
        FAULT = 7,
        PING = 8,
        PONG = 9,
        AUTH = 10
} broker_action_type , t_broker_action_type;


const char* broker_action_type_names[] = {
            "PUBLISH",
            "POLL",
            "ACCEPTED",
            "ACKNOWLEDGE_MESSAGE",
            "SUBSCRIBE",
            "UNSUBSCRIBE",
            "NOTIFICATION",
            "FAULT",
            "PING",
            "PONG",
            "AUTH"
  };

typedef struct{

	char* action_id;

} broker_ping_action;

typedef struct{

	char* action_id;

} broker_pong_action;


typedef struct{

    t_broker_action_type action_type;

    broker_ping_action *action_ping;
    broker_pong_action *action_pong;


} broker_message , t_broker_message;



t_broker_message *json_to_message(char *json_data);
broker_ping_action *json_to_ping_action(json_t* json_action);
broker_pong_action *json_to_pong_action(json_t* json_action);

char* broker_get_action_type_name(broker_action_type type);
char* broker_get_action_id(t_broker_message* message);


t_broker_message *xml_to_message(char *xml_data,int len);


#endif
