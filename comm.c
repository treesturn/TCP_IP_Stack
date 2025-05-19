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

static char recv_buffer[MAX_PACKET_BUFFER_SIZE];
static void _pkt_receive(node_t* receving_node, 
			 char* pkt_with_aux_data,
			 unsigned int pkt_size){
}


static unsigned int udp_port_number = 40000;

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

static void* _network_start_pkt_receiver_thread(void* arg){
	node_t* node;
	glthread_t* curr;

	fd_set active_sock_fd_set,
	       backup_sock_fd_set;

	int sock_max_fd = 0;
	int bytes_recvd = 0;

	graph_t* topo = (void *)arg;

	int addr_len = sizeof(struct sockaddr);

	FD_ZERO(&active_sock_fd_set);
	FD_ZERO(&backup_sock_fd_set);

	struct sockaddr_in sender_addr;

	ITERATE_GLTHREAD_BEGIN(&topo->node_list, curr){
		node = graph_glue_to_node(curr);

		if(!node->udp_sock_fd){
			continue;
		}

		if(node->udp_sock_fd, &backup_sock_fd_set){
			sock_max_fd = node->udp_sock_fd;
		}

		FD_SET(node->udp_sock_fd, &backup_sock_fd_set);
	} ITERATE_GLTHREAD_END(&topo->node_list, curr)
	
	while(1){
		memcpy(&active_sock_fd_set, &backup_sock_fd_set, sizeof(fd_set));
		select(sock_max_fd+1, &active_sock_fd_set, NULL, NULL, NULL);

		ITERATE_GLTHREAD_BEGIN(&topo->node_list, curr){
			node = graph_glue_to_node(curr);
			
			if(FD_ISSET(node->udp_sock_fd, &active_sock_fd_set)){
				memset(recv_buffer, 0, MAX_PACKET_BUFFER_SIZE);
				bytes_recvd = recvfrom(node->udp_sock_fd, (char *)recv_buffer, 
							MAX_PACKET_BUFFER_SIZE, 0, 
							(struct sockaddr *)&sender_addr, &addr_len);
				_pkt_receive(node, recv_buffer, bytes_recvd);
			}

		}ITERATE_GLTHREAD_END(&topo->node_list, curr)
	}
}

void network_start_pkt_receiver_thread(graph_t* topo){
	pthread_attr_t attr;
	pthread_t recv_pkt_thread;

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

	pthread_create(&recv_pkt_thread, &attr, _network_start_pkt_receiver_thread, (void *)topo);

}
