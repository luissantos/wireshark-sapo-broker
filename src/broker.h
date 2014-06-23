#ifndef SAPO_BROKER_H
#define SAPO_BROKER_H

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

    t_broker_action_type action_type;

} broker_message , t_broker_message;


t_broker_message *json_to_message(char *json_data);

char* broker_get_action_type_name(broker_action_type type);


#endif
