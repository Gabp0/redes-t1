#include "connection.h"
#include <string>
#include <cstring>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdexcept>
#include <iostream>
#include "sockets/socket.h"
#include "githyanki.h"

using namespace std;
using namespace Githyanki;

Connection::Connection(string device)
{
    char *device_c = new char[device.length() + 1];
    strcpy(device_c, device.c_str());
    this->socket = cria_raw_socket(device_c);
}

Connection::~Connection(void)
{
    close(this->socket);
}

int Connection::receiveMessage(int timeoutMillis, char *buffer, int tamanho_buffer)
{
    long long comeco = timestamp();
    struct timeval timeout = {.tv_sec = 0, .tv_usec = timeoutMillis * 1000};
    setsockopt(this->socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout));
    int bytes_lidos;
    do
    {
        bytes_lidos = recv(this->socket, buffer, tamanho_buffer, 0);
        if (Githyanki::isValid(buffer, bytes_lidos))
        {
            return bytes_lidos;
        }
    } while (timestamp() - comeco <= timeoutMillis);
    return -1;
}

void Connection::sendMessage(frame msg)
{
    //cout << this->socket << endl;
    char bytes[FRAME_SIZE_MAX];
    size_t size = msg.toBytes(bytes);

    printf("%s\n%ld\n", bytes, size);

    ssize_t sent = send(this->socket, bytes, size, 0);
    if (sent > 0)
    {
        cout << "sent" << endl;
    }
}

int Connection::Acknowledge(int sequence){
    struct frame awk = frame(AWK, sequence);
    char bytes[FRAME_SIZE_MAX];

    size_t size = awk.toBytes(bytes);

    ssize_t sent = send(this->socket, bytes, size, 0);
    return sent;
}

long long Connection::timestamp() {
    struct timeval tp;
    gettimeofday(&tp, NULL);
    return tp.tv_sec*1000 + tp.tv_usec/1000;
}