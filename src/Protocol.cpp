#include "Protocol.h"
#include "../sockets/socket.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>
#include <cstring>

using namespace std;

Protocol::Protocol(char *device)
{
    this->socket = cria_raw_socket(device);
    cout << this->socket << endl;
}

// retorna -1 se deu timeout, ou quantidade de bytes lidos
int Protocol::receiveMessage(int timeoutMillis, char *buffer, int tamanho_buffer)
{
    long long comeco = timestamp();
    struct timeval timeout = {.tv_sec = 0, .tv_usec = timeoutMillis * 1000};
    setsockopt(this->socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout));
    int bytes_lidos;
    do
    {
        bytes_lidos = recv(this->socket, buffer, tamanho_buffer, 0);
        if (protocolo_e_valido(buffer, bytes_lidos))
        {
            return bytes_lidos;
        }
    } while (timestamp() - comeco <= timeoutMillis);
    return -1;
}

void Protocol::sendMessage(char *message)
{
    ssize_t sent = send(socket, (void *)message, strlen(message), 0);
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
