#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>


#include "../sockets/socket.h"
#include "string.h"
#include "server.h"

void server()
{
    struct sockaddr client_info = {0};
    socklen_t client_info_len = sizeof(client_info);

    //Cria socket
    int socket = -1;
    char rede[] = "lo";
    socket = cria_raw_socket(rede);

    //Listen
    //if(0 > listen(socket, 0)){
    //    perror("listen");
    //    return;
    //}

    //Accept
    // int cfd = accept(socket, &client_info, &client_info_len);
    // if(0 > cfd){
    //     perror("accept");
    //     return;
    // }

    //Send
    char *hello = "Hello World";
    ssize_t sent = send(socket, (void *)hello, strlen(hello), 0);
    
    // close(cfd);
}