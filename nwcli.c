#include "CommandParser/libcli.h"
#include "CommandParser/cmdtlv.h"
#include "cmdcodes.h"
#include "graph.h"
#include <stdio.h>

extern graph_t* topo;

static int show_nw_topology_handler(param_t* param, ser_buff_t* tlv_buff, op_mode enable_or_disable){

	int CMDCODE = -1;
	CMDCODE = EXTRACT_CMD_CODE(tlv_buff);

	switch(CMDCODE){
		case CMDCODE_SHOW_NW_TOPOLOGY:
			dump_nw_graph(topo);
			break;
		default:
			;
	}
}

void nw_init_cli(){
	
	init_libcli();

	param_t* show = libcli_get_show_hook();
	param_t* debug = libcli_get_debug_hook();
	param_t* config = libcli_get_config_hook();
	param_t* run = libcli_get_run_hook();
	param_t* debug_show = libcli_get_debug_show_hook();
	param_t* root = libcli_get_root();

	{
		//show topology custom command
		static param_t topology;
		init_param(&topology, CMD, "topology", show_nw_topology_handler, 0, INVALID, 0, 
				"Dump Complete Network Topology");
		libcli_register_param(show, &topology);
		set_param_cmd_code(&topology, CMDCODE_SHOW_NW_TOPOLOGY);
	}
	support_cmd_negation(config);
}
