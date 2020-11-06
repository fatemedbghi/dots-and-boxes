#include "client.h"

int main(int argc , char *argv[])
{
    server_port = atoi(argv[1]);
    siginterrupt(SIGALRM, 1);
    connect_to_server();
    return 0;
}