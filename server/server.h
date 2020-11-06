#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>  
#include <string.h>
#include <stdlib.h>  
#include <errno.h>  
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <sys/time.h>
#include <netdb.h>
#include <signal.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>

#define TRUE   1  
#define FALSE  0  
#define SERVER_PORT 8080
#define SERVER_ADDRESS "127.0.0.1"

int server_socket;
extern int max_clients;
struct sockaddr_in address;
int client_sockets[30];
int player2[2];
int player3[3];
int player4[4];
int temp_port;
fd_set readfds;
int port;


void server_socket_init();
void incoming_connections();
void incoming_input();
void check_group(int k, int val);
void make_group(int val);
char* itoa(int input, char* str);

#endif