#include "protocol.h"
#include "sockets/socket.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>
#include <cstring>
#include <netinet/in.h>

using namespace std;

int protocol::initSocket(string device)
{
    char *device_c = new char[device.length() + 1];
    strcpy(device_c, device.c_str());
    int socket = cria_raw_socket(device_c);
    cout << socket << endl;
    return socket;
}

// retorna -1 se deu timeout, ou quantidade de bytes lidos
int protocol::receiveMessage(int socket, int timeoutMillis, char *buffer, int tamanho_buffer)
{
    long long comeco = timestamp();
    struct timeval timeout = {.tv_sec = 0, .tv_usec = timeoutMillis * 1000};
    setsockopt(socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout));
    int bytes_lidos;
    do
    {
        bytes_lidos = recv(socket, buffer, tamanho_buffer, 0);
        if (protocol::isValid(buffer, bytes_lidos))
        {
            return bytes_lidos;
        }
    } while (timestamp() - comeco <= timeoutMillis);
    return -1;
}

void protocol::sendMessage(frame )
{
    char *msg = msg.toChar();
    ssize_t sent = send(socket, , strlen(frame), 0);
}

protocol::frame *protocol::createFrame(string message, unsigned short type, unsigned short seq)
{
    protocol::frame *f = new protocol::frame;
    f->mark = protocol::SOH;
    f->type = type;
    f->seq = seq;
    f->size = sizeof(message);
    f->checksum = 0x12;

    for (int i = 0; i < sizeof(*f); i++)
    {
        printf("%c", ((char *)f)[i]);
    }
}

// usando long long pra (tentar) sobreviver ao ano 2038
long long protocol::timestamp()
{
    struct timeval tp;
    gettimeofday(&tp, NULL);
    return tp.tv_sec * 1000 + tp.tv_usec / 1000;
}

int protocol::isValid(char *buffer, int tamanho_buffer)
{
    if (tamanho_buffer <= 0)
    {
        return 0;
    }
    // insira a sua validação de protocolo aqui
    return buffer == "0x7f"; // Esta errado
}
