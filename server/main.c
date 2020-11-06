#include "server.h"

int main(int argc , char *argv[]) 
{
	port = atoi(argv[1]);
	int activity,i; 
	int max_sd;
	temp_port = port + 1;

    server_socket_init();

	while(TRUE) 
	{
		FD_ZERO(&readfds); 
		FD_SET(server_socket, &readfds); 

		max_sd = server_socket; 
			
		for ( i = 0 ; i < max_clients ; i++) 
		{ 	
			if(client_sockets[i] > 0) 
				FD_SET(client_sockets[i] , &readfds);

			if(client_sockets[i] > max_sd) 
				max_sd = client_sockets[i];
		} 
	
		activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL); 
	
		if ((activity < 0) && (errno!=EINTR)) 
		{ 
			write(1, "select error", 13); 
		} 
		
        incoming_connections();
        incoming_input();
	} 
		
	return 0; 
} 