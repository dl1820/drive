#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "find_link_module.h"

// mysql
#define DB_HOST "localhost"
#define DB_USER "root"
#define DB_PASS "1"
#define DB_NAME "drive"
#define CHOP(x) x[strlen(x) - 1] = ""

// socket
#define BUFF_SIZE 65536

void finish_with_error(MYSQL *conn);

typedef struct _data {
      MYSQL_RES * gps;
        int num;
} data;

typedef struct _recv_location{
	double lat;
	double lon;
	int head;
}

int main(void)
{
    // socket
    int server_socket;
    int client_socket;
    int client_addr_size;

    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;

    char buff_rcv[BUFF_SIZE];
    char buff_snd[BUFF_SIZE];
    
    server_socket = socket( PF_INET, SOCK_STREAM, 0 );
    if (server_socket == -1)
    {
        printf("server socket create faile\n");
        exit(1);    
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(40000);
    server_addr.sin_addr.s_addr = htons(INADDR_ANY);

    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1)
    {
        printf("bind execution faile\n");
        exit(1);    
    }

    if (listen(server_socket, 5) == -1)
    {
        printf("listen execution faile\n");
        exit(1);
    }

    while(1)
    {
        client_addr_size = sizeof(client_addr);
        client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_addr_size);
        if ( client_socket == -1)
        {
            printf("client connection faile\n");
            exit(1);
        }

        read(client_socket, buff_rcv, BUFF_SIZE);
        printf("receive : %s\n", buff_rcv);

        write(client_socket, buff_snd, strlen(buff_snd));
        close(client_socket);
    }
}

