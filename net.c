#include "graph.h"
#include <memory.h>
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>


bool_t node_set_loopback_address(node_t* node, char* ip_addr){
	assert(ip_addr);

	node->node_nw_prop.is_lb_addr_config = TRUE;
	//NODE_LO_ADDR(node) macro that retrieves node's loopback address
	strncpy(NODE_LO_ADDR(node), ip_addr, 16);
	NODE_LO_ADDR(node)[15] = '\0'
}

bool_t node_set_intf_ip_address(node_t* node, char* local_if, char* ip_addr, char mask){
	interface_t* interface = get_node_if_by_name(node, local_if);
	if (!interface) assert(0);

	//IF_IP(interface) macro that retrieves interface's ip address
	strncpy(IF_IP(interface), ip_addr, 16);
	IF_IP(interface)[15] = '\0';
	interface->intf_nw_props.mask = mask;
	interface->intf_nw_props.ip_ipadd_config = TRUE;
	return TRUE;

}

bool_t node_unset_intf_ip_address(node_t* node, char* local_if){
	return TRUE;
}
