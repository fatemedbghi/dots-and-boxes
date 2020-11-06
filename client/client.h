#ifndef CLIENT_H
#define CLIENT_H

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>


#define TRUE   1  
#define FALSE  0  
#define CLIENT_ADDRESS "255.255.255.255"
#define TIMEOUT 20
#define PORT 8080
#define SERVER_ADDRESS "127.0.0.1"


int client_socket;
int client_to_sever;
struct sockaddr_in client_address;
struct sockaddr_in server_address;
int port;
int group_n;
int id;
char buffer[1024];
int turn;
int hmap[5][4];
int vmap[4][5];
int score;
int move;
int server_port;

void connect_to_server();
void create_new_socket();
bool game();
char* itoa(int input, char* str);
void update_map(char* temp_buff, int flag);
void calc_turn(int square);
int look_for_completed_square(char* direction,int x, int y);
bool check_for_winner();
void print_game();
void sig_handler(int signum);

#endif