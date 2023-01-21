#include <iostream>
#include "sockets/socket.h"
#include "sockets/ConexaoRawSocket.h"
#include <sys/socket.h>
#include "client/client.h"
#include "server/server.h"
#include <string.h>

#include <net/if.h>

int main(int argc, const char** argv) {

    if(argc == 2){
        if(strcmp(argv[1], "s") == 0){
            std::cout << "server" << std::endl;
            server();
        }
        if(strcmp(argv[1], "c") == 0){
            std::cout << "client" << std::endl;
            //client();
        }
    }

    return 0;
}