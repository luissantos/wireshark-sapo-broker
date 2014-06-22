/* #include "config.h"*/

#include <config.h>    
#include <epan/packet.h>

#define BROKER_PORT 3323

static int proto_broker = -1;


void proto_register_broker(void)
{
    proto_broker = proto_register_protocol (
        "Broker Protocol", /* name       */
        "Boker",      /* short name */
        "broker"       /* abbrev     */
        );
}

static void dissect_broker(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree)
{
    col_set_str(pinfo->cinfo, COL_PROTOCOL, "BROKER");
    /* Clear out stuff in the info column */
    col_clear(pinfo->cinfo,COL_INFO);
}

void proto_reg_handoff_broker(void)
{
    static dissector_handle_t broker_handle;

    broker_handle = create_dissector_handle(dissect_broker, proto_broker);
    dissector_add_uint("tcp.port", BROKER_PORT, broker_handle);
}



