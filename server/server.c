#include "server.h"

int max_clients = 30;

char* itoa(int input, char* str) {
	int temp2 = input;
    int num = 0;
    while (temp2)
    {
      temp2 = temp2 / 10;
      num++;
    }
    int temp3 = input;
    for(int i = 0; i < num; i++)
    {
      str[num - i - 1] = '0' + (temp3 % 10);
      temp3 = temp3 / 10;
    }
	str[num] = '\0';
	return str;
}

void server_socket_init(){
    
    int i;
    for (i = 0; i < max_clients; i++) 
	{ 
		client_sockets[i] = 0; 
	}

    if( (server_socket = socket( AF_INET , SOCK_STREAM, 0)) < 0 )
    {
        write(1, "socket failed", 14);
        exit(EXIT_FAILURE); 
    }

    int opt = TRUE;
    if( setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 )   
    {   
        write(1, "setsockopt", 11);
        exit(EXIT_FAILURE);   
    }

    address.sin_family = AF_INET; 
	address.sin_addr.s_addr = inet_addr(SERVER_ADDRESS);
	address.sin_port = htons(port); 
    
    if (bind(server_socket, (struct sockaddr *)&address, sizeof(address))<0) 
	{ 
        write(1,"bind failed", 12);
		exit(EXIT_FAILURE); 
	}
    
    if (listen(server_socket, 30) < 0) 
	{ 
		write(1,"listen failed", 14);
		exit(EXIT_FAILURE); 
	} 

}

void incoming_connections(){

    int new_socket, i, addrlen;
    struct sockaddr_in addr;
    addrlen = sizeof(addr);
    memset(&addr, 0, sizeof(addr));

    if (FD_ISSET(server_socket, &readfds)) 
	{ 
		if ((new_socket = accept(server_socket, (struct sockaddr *)&addr, (socklen_t*)&addrlen))<0) 
		{ 
			write(1,"accept failed", 14);
			exit(EXIT_FAILURE); 
		} 
	
		char *message = "You are connected to server!\n";   
		if(send(new_socket, message, strlen(message), 0) != strlen(message)) 
		{ 
			write(1, "send failed",12);
		} 

        write(1, "Welcome message sent successfully\n", 35);
		
		for (i = 0; i < max_clients; i++) 
		{
			if( client_sockets[i] == 0 ) 
			{
				client_sockets[i] = new_socket; 
				break; 
			} 
		} 
	}

}

void make_group(int val){
	
	char str[50];
	if (val == 2){
		for (int j = 0; j < val; j++) {
			itoa(temp_port*10 + j+1, str);
			if (send(player2[j] , str , strlen(str) , 0 ) < 0) {
				write(1,"send failed\n",13);
				return ;
			}
			player2[j] = 0;
		}
	}
	if (val == 3){
		for (int j = 0; j < val; j++) {
			itoa(temp_port*10 + j+1, str);
			if (send(player3[j] , str , strlen(str) , 0 ) < 0) {
				write(1,"send failed\n",13);
				return ;
			}
			player3[j] = 0;
		}
	}
	if (val == 4){
		for (int j = 0; j < val; j++) {
			itoa(temp_port*10 + j+1, str);
			if (send(player4[j] , str , strlen(str) , 0 )  < 0) {
				write(1,"send failed\n",13);
				return ;
			}
			player4[j] = 0;
		}
	}
	temp_port += 1;
}

void check_group(int k, int val) {
	
	if (val == 2) {
		if (player2[0] == 0) {
			player2[0] = client_sockets[k];
		}
		else {
			player2[1] = client_sockets[k];
			make_group(val);
		}
	}

	if (val == 3) {
		if (player3[0] == 0) {
			player3[0] = client_sockets[k];
		}
		else {
			if (player3[1] == 0) {
				player3[1] = client_sockets[k];
			}
			else {
				player3[2] = client_sockets[k];
				make_group(val);
			}
		}
	}

	if (val == 4) {
		if (player4[0] == 0) {
			player4[0] = client_sockets[k];
		}
		else {
			if (player4[1] == 0) {
				player4[1] = client_sockets[k];
			}
			else {
				if (player4[2] == 0) {
					player4[2] = client_sockets[k];
				}
				else {
					player4[3] = client_sockets[k];
					make_group(val);
				}
			}
		}

	}
}

void incoming_input(){

    int new_socket, i, addrlen;
    struct sockaddr_in addr;
    addrlen = sizeof(addr);
    memset(&addr, 0, sizeof(addr));
	
	for (i = 0; i < max_clients; i++) 
	{
		if (FD_ISSET( client_sockets[i] , &readfds)) 
		{ 
			int valread;
            char buffer[256];

			valread = recv(client_sockets[i] , buffer, sizeof(buffer),0);
			write(1,"Number of game players received\n",33);
			buffer[valread] = '\0'; 
			int val;
   			val = atoi(buffer);
			check_group(i,val);
			client_sockets[i] = 0;
		} 
	} 
}