#include "client.h"

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

void print_game() {
    for (int j=0;j<group_n;j++) {
        for (int k=0;k<group_n;k++)
        {
            write(1,"*",2);
            if (hmap[j][k] == 1) write(1,"--",2);
            else write(1,"  ",2);
        }
        write(1,"*\n",3);
        for (int k=0;k<=group_n;k++)
        {
            if (vmap[j][k] == 1) write(1,"|  ",4);
            else write(1,"   ",4);
        }
        write(1,"\n",2);
    }
    for (int k=0;k<group_n;k++)
    {
        write(1,"*",2);
        if (hmap[group_n][k] == 1) write(1,"--",2);
        else write(1,"  ",2);
    }
    write(1,"*\n",3);
}

int look_for_completed_square(char* direction,int x, int y) {
    int n = 0;
    if (strcmp("U",direction) == 0) {
        if (x != 0) {
            if(hmap[x-1][y] == 1 && vmap[x-1][y] == 1 && vmap[x-1][y+1] == 1) n++;
        }
        if(hmap[x+1][y] == 1 && vmap[x][y] == 1 && vmap[x][y+1] == 1) n++;
    }
    if (strcmp("D",direction) == 0) {
        if (x != group_n - 1) {
            if(hmap[x+2][y] == 1 && vmap[x+1][y] == 1 && vmap[x+1][y+1] == 1) n++;
        }
        if(hmap[x][y] == 1 && vmap[x][y] == 1 && vmap[x][y+1] == 1) n++;
    }
    if (strcmp("R",direction) == 0) {
        if (y != group_n - 1) {
            if (vmap[x][y+2] == 1 && hmap[x][y+1] == 1 && hmap[x+1][y+1]) n++;
        }
        if (vmap[x][y] == 1 && hmap[x][y] == 1 && hmap[x+1][y]) n++;
    }
    if (strcmp("L",direction) == 0) {
        if (y != 0) {
            if (vmap[x][y-1] == 1 && hmap[x][y-1] == 1 && hmap[x+1][y-1]) n++;
        }
        if (vmap[x][y+1] == 1 && hmap[x][y] == 1 && hmap[x+1][y]) n++;
    }
    return n;
}

void calc_turn(int square) {

    if (group_n == 2) {
        if (square == 1) {
            write(1,"--> You made a square! <--\n",28);
            if (id == 1) buffer[0] = '1';
            else buffer[0] = '2';
        }
        else {
            if (id == 1) buffer[0] = '2';
            else buffer[0] = '1';
        }
    }
    if (group_n == 3) {
        if (square == 1) {
            if (id == 1) buffer[0] = '1';
            else if(id == 2) buffer[0] = '2';
            else buffer[0] = '3';
        }
        else {
            if (id == 1) buffer[0] = '2';
            else if(id == 2) buffer[0] = '3';
            else buffer[0] = '1';
        }
        
    }
    if (group_n == 4)
    {
        if (square == 1) {
            if (id == 1) buffer[0] = '1';
            else if(id == 2) buffer[0] = '2';
            else if(id == 3) buffer[0] = '3';
            else buffer[0] = '4';
        }
        else {
            if (id == 1) buffer[0] = '2';
            else if(id == 2) buffer[0] = '3';
            else if(id == 3) buffer[0] = '4';
            else buffer[0] = '1';
        }
    }
}

void update_map(char* temp_buff, int flag) {
    char* token = strtok(temp_buff, "-"); 
    int j = -1;
    char* direction;
    int line[2];
    int square = 0;
    while (token != NULL) {
        if (j == 0) {
            direction = token;
        }
        if (j == 1) {
            line[0] = atoi(token);
        }
        if (j == 2){
            line[1] = atoi(token);
        }
        token = strtok(NULL, "-"); 
        j++;
    }
    if (line[0] != 9 && line[1] != 9)
    {
        if (strcmp("U", direction) == 0) hmap[line[0]][line[1]] = 1; 
        if (strcmp("D", direction) == 0) hmap[line[0]+1][line[1]] = 1; 
        if (strcmp("R", direction) == 0) vmap[line[0]][line[1]+1] = 1;
        if (strcmp("L", direction) == 0) vmap[line[0]][line[1]] = 1;
        square = look_for_completed_square(direction,line[0],line[1]);  
    }
    else move--;

    if (flag == 1) {
        score = score + square;
        calc_turn(square);
    }

}

bool check_for_winner(){

    if (group_n == 2 && score >= 3) return 1;
    if (group_n == 3 && score >= 5) return 1;
    if (group_n == 4 && score >= 9) return 1;

    return 0;
}

void sig_handler(int signum){

    write(1,"You missed your turn :(\n",25);

}

bool game()
{
    char temp_buff[1024];
    memset(buffer, 0, sizeof(buffer));
    int flag;
    if (turn == id) {
        flag = 1;
        buffer[0]='U';
        buffer[1]='-';
        buffer[2]='9';
        buffer[3]='-';
        buffer[4]='9';
        write(1,"Your turn\n",11);
        signal(SIGALRM,sig_handler);
        alarm(TIMEOUT);
        read(0, buffer, 1024);
        alarm(0);
        memmove(buffer+2, buffer, 5);
        buffer[1] = '-';
        memset(temp_buff, 0, sizeof(temp_buff));
        strncpy(temp_buff,buffer, 1024);
        update_map(temp_buff,flag);
        int winner = check_for_winner();
        if (winner == 1 && move == 2*group_n*(group_n+1)-1) {
            sendto(client_socket,"loseeer", 7, 0 ,(struct sockaddr *)&client_address,  sizeof(client_address));
            write(1,"You won the game!!!\n",20);
            return 1;
        }
        if (sendto(client_socket,buffer, 1024, 0 ,(struct sockaddr *)&client_address,  sizeof(client_address)) < 0){
            write(1,"send failed\n",13);
            return 0;
        }
        recv(client_socket,buffer,1024,0);
        write(1,"New line sent!\n",15);
        
    }
    else
    {
        flag = 0;
        write(1,"Wait for other player...\n",26);
        int n;
        if (n = recv(client_socket,buffer,1024, 0) < 0){
            write(1,"recieve failed\n",16);
            return 0;
        }
        if (move == 2*group_n*(group_n+1)-1 && strcmp("loseeer",buffer) == 0) {
            write(1,"You lost the game!!!\n",22);
            return 1;
        }
        buffer[7] = '\0';
        write(1,"New line recieved\n",19);
        memset(temp_buff, 0, sizeof(temp_buff));
        strncpy(temp_buff,buffer, 1024);
        update_map(temp_buff,flag);   
    }
    move++;
    print_game();
    turn = atoi(&buffer[0]);
    if (move == 2*group_n*(group_n+1)) {
        write(1,"Both win the game!!!\n",21);
        return 1;
    }
    return 0;
}

void create_new_socket(){
    close(client_to_sever);
    int broadcast = 1;
    int opt = 1;

    if ((client_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        write (1, "fail to open socket", 20);
        return ;
    }
    if (setsockopt(client_socket, SOL_SOCKET, SO_BROADCAST, &broadcast,sizeof(broadcast)) < 0) {
        write (1,"setsockopt (SO_BROADCAST)", 26);
        return ;
    }

    write(1,"UDP\n",5);

    client_address.sin_family = AF_INET;
    client_address.sin_addr.s_addr = inet_addr(CLIENT_ADDRESS);
    client_address.sin_port = htons(port);
    memset(client_address.sin_zero, '\0', sizeof client_address.sin_zero);

    setsockopt(client_socket, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));

    if ( bind(client_socket, (const struct sockaddr *)&client_address,  sizeof(client_address)) < 0 ) 
    { 
        write (1, "bind failed", 12);
        return ;
    }

    turn = 1;
    score = 0;
    move = 0;
    int winner;
    while (TRUE)
    {
        winner = game();
        if (winner == 1) break;
    }

}

void connect_to_server()
{
    
    if( (client_to_sever = socket( AF_INET , SOCK_STREAM, 0)) < 0 )
    {
        write (1, "fail to open socket", 20);
        return ;
    }

    server_address.sin_family = AF_INET; 
	server_address.sin_port = htons(server_port); 

    if(inet_pton(AF_INET,SERVER_ADDRESS, &server_address.sin_addr)<=0)  
    { 
        write (1, "Invalid address\n", 17);
        return ; 
    }

    if (connect(client_to_sever, (struct sockaddr*)&server_address,sizeof(server_address)) < 0) {
		write(1, "could not connect to server\n", 29);
        return ;
	}
    
    
    char server_response[256];
    recv(client_to_sever,&server_response,sizeof(server_response),0);
    printf("%s",server_response);

    char number[256];
    write(1, "Enter number of players in the game\n", 37);
    read(0, number, 256);
    group_n = atoi(number);
    if (send(client_to_sever, number,  strlen(number), 0) <= 0)
    {
        write (1, "send failed\n", 13);
        return ; 
    }
    char server_response1[256];
    recv(client_to_sever,&server_response1,sizeof(server_response1),0);
    int temp_out = atoi(server_response1);
    id = temp_out%10;
    port = temp_out/10;
    char port_s[50];
    char client_id[50];
    itoa(port,port_s);
    itoa(id,client_id);
    write(1,"Given port is: ", 16);
    write(1,port_s,strlen(port_s));
    write(1,"\n",2);
    write(1,"client id is: ", 15);
    write(1,client_id,strlen(client_id));
    write(1,"\n",2);
    create_new_socket();
}