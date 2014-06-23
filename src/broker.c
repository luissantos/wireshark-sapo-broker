/* #include "config.h"*/

#include <apr.h>
#include <apr_general.h>
#include <apr_pools.h>

#include "broker.h"
#include <config.h>
#include <epan/packet.h>
#include <jansson.h>
#include <stdio.h>
#include <stdlib.h>




#define BROKER_PORT 3323



static int proto_broker = -1;

static int hf_broker_encoding_type = -1;
static int hf_broker_encoding_version = -1;
static int hf_broker_message_size = -1;
static int hf_broker_action_type = -1;
static gint ett_broker = -1;

apr_pool_t *pool = NULL;

void proto_register_broker(void)
{

    static hf_register_info hb[] = {

        { &hf_broker_encoding_type,
            { "Encoding Type", "broker.encoding",
            FT_UINT16, BASE_DEC,
            NULL, 0x0,
            NULL, HFILL }
        },
         { &hf_broker_encoding_version,
            { "Encoding Version", "broker.encoding_version",
            FT_UINT16, BASE_DEC,
            NULL, 0x0,
            NULL, HFILL }
        },
        { &hf_broker_message_size,
            { "Size", "broker.size",
            FT_UINT32, BASE_DEC,
            NULL, 0x0,
            NULL, HFILL }
        },
         { &hf_broker_action_type,
            { "Action Type", "broker.action.type",
            FT_UINT_STRING, BASE_NONE,
            NULL, 0x0,
            NULL, HFILL }
        },
    };

    /* Setup protocol subtree array */
    static gint *ett[] = {
        &ett_broker
    };


    /* Initialize APR at the program start */
  apr_initialize();

  /* atexit(apr_terminate()); gives
   * error: invalid use of void expression
   */
  atexit(apr_terminate);

  /* create a managed memory pool with APR */
  apr_pool_create(&pool, NULL);



    proto_broker = proto_register_protocol (
        "Broker Protocol", /* name       */
        "Boker",      /* short name */
        "broker"       /* abbrev     */
        );

    proto_register_field_array(proto_broker, hb, array_length(hb));
    proto_register_subtree_array(ett, array_length(ett));
}


static void dissect_broker(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree)
{
    col_set_str(pinfo->cinfo, COL_PROTOCOL, "BROKER");
    /* Clear out stuff in the info column */
    col_clear(pinfo->cinfo,COL_INFO);


    if(tree){
        proto_item *ti = NULL;
        proto_tree *broker_tree;

        ti = proto_tree_add_item(tree,proto_broker,tvb,0,-1,ENC_NA);

        broker_tree = proto_item_add_subtree(ti,ett_broker);
        proto_tree_add_item(broker_tree, hf_broker_encoding_type, tvb, 0, 2, ENC_BIG_ENDIAN);
        proto_tree_add_item(broker_tree, hf_broker_encoding_version, tvb, 2, 2, ENC_BIG_ENDIAN);
        proto_tree_add_item(broker_tree, hf_broker_message_size, tvb, 4, 4, ENC_BIG_ENDIAN);



        int len =  tvb_length(tvb) - 8;

        char *data = (char*)tvb_get_ptr(tvb,8,len);

        printf("%s",data);

        t_broker_message* message;

        message = json_to_message(data);

        char* type = broker_get_action_type_name(message->action_type);

        proto_tree_add_text(broker_tree,tvb,8,len, type);
    }


}



t_broker_message *json_to_message(char *json_data){

        json_error_t error;

        json_t *root;

        t_broker_message* message = apr_palloc(pool, sizeof(t_broker_message));

        root = json_loads(json_data, 0, &error);

        char *action_type = json_string_value(json_object_get(json_object_get(root,"action"),"action_type"));


        if(strcmp(action_type,"PING") == 0){
            message->action_type = PING;
        }
        if(strcmp(action_type,"PONG") == 0){
            message->action_type = PONG;
        }

        return message;
}


void proto_reg_handoff_broker(void)
{
    static dissector_handle_t broker_handle;

    broker_handle = create_dissector_handle(dissect_broker, proto_broker);
    dissector_add_uint("tcp.port", BROKER_PORT, broker_handle);
}

char* broker_get_action_type_name(broker_action_type type){

    return broker_action_type_names[type];

}


