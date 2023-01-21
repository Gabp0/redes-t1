#include "Protocol.h"
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

Protocol::Protocol(string device)
{
    char *device_c = new char[device.length() + 1];
    strcpy(device_c, device.c_str());
    this->socket = cria_raw_socket(device_c);
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
        if (Protocol::isValid(buffer, bytes_lidos))
        {
            return bytes_lidos;
        }
    } while (timestamp() - comeco <= timeoutMillis);
    return -1;
}

void Protocol::sendMessage(char* message, char seq)
{
    // frame f = Protocol::createFrame(message, seq);

    ssize_t sent = send(socket, (void *)message, strlen(message), 0);
}

Protocol::frame *Protocol::createFrame(string message, unsigned short type, unsigned short seq)
{
    Protocol::frame *f = new Protocol::frame;
    f->mark = Protocol::MARK;
    f->type = type;
    f->seq = seq;
    f->size = sizeof(message);
    f->checksum = 0x12;

    for (int i = 0; i < sizeof(*f); i++)
    {
        printf("%c", ((char *)f)[i]);
    }
}

unsigned short Protocol::checksum(unsigned short *buff, int _16bitword)
{
    unsigned long sum;
    for (sum = 0; _16bitword > 0; _16bitword--)
        sum += htons(*(buff)++);
    sum = ((sum >> 16) + (sum & 0xFFFF));
    sum += (sum >> 16);
    return (unsigned short)(~sum);
}

// usando long long pra (tentar) sobreviver ao ano 2038
long long Protocol::timestamp()
{
    struct timeval tp;
    gettimeofday(&tp, NULL);
    return tp.tv_sec * 1000 + tp.tv_usec / 1000;
}

int Protocol::isValid(char *buffer, int tamanho_buffer)
{
    if (tamanho_buffer <= 0)
    {
        return 0;
    }
    // insira a sua validação de protocolo aqui
    return buffer[0] == '0x7f';
}
