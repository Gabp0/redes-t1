#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>
#include "../sockets/socket.h"
#include <iostream>
#include "string.h"
#include "client.h"

void client()
{
    // Cria socket
    int socket = -1;
    char rede[] = "lo";
    socket = cria_raw_socket(rede);

    char buffer[256];

    if (recebe_mensagem(socket, 10000, buffer, 255) != -1)
    {
        std::cout << "Recived:" << buffer << std::endl;
    }
    else
    {
        std::cout << "Timeout" << std::endl;
    }
}

// usando long long pra (tentar) sobreviver ao ano 2038
long long timestamp()
{
    struct timeval tp;
    gettimeofday(&tp, NULL);
    return tp.tv_sec * 1000 + tp.tv_usec / 1000;
}

int protocolo_e_valido(char *buffer, int tamanho_buffer)
{
    if (tamanho_buffer <= 0)
    {
        return 0;
    }
    // insira a sua validação de protocolo aqui
    return buffer[0] == '0x7f';
}

// retorna -1 se deu timeout, ou quantidade de bytes lidos
int recebe_mensagem(int soquete, int timeoutMillis, char *buffer, int tamanho_buffer)
{
    long long comeco = timestamp();
    struct timeval timeout = {.tv_sec = 0, .tv_usec = timeoutMillis * 1000};
    setsockopt(soquete, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout));
    int bytes_lidos;
    do
    {
        bytes_lidos = recv(soquete, buffer, tamanho_buffer, 0);
        if (protocolo_e_valido(buffer, bytes_lidos))
        {
            return bytes_lidos;
        }
    } while (timestamp() - comeco <= timeoutMillis);
    return -1;
}