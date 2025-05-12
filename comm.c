#include "comm.h"
#include "graph.h"
#include <sys/socket.h>
#include <pthread.h>
#include <netinet/in.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <netdb.h>

static unsigned int udp_port_numebr = 40000;

static unsigned int get_next_udp_port_number(){
	return udp_port_number++;
}

void init_udp_socket(node_t* node){
	node->udp_port_number = get_next_udp_port_number();
	
	int udp_sock_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	// socket binding
	struct sockaddr_in node_addr;
	node_addr.sin_family = AF_INET;
	node_addr.sin_port = node->udp_port_number;
	node_addr. sin_addr.s_addr = INADDR_ANY;
	if (bind(udp_sock_fd, (struct sockaddr* )&node_addr, sizeof(struct sockaddr)) == -1){
		printf("Error : socket bind failed for Node %s\n", node->node_name);
		return;
	}
	node->udp_sock_fd = udp_sock_fd;

}


