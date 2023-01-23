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
#include "sockets/socket.h"
#include "githyanki.h"

using namespace std;

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

void Connection::sendMessage(void *msg, size_t size)
{
    ssize_t sent = send(this->socket, msg, size, 0);

    if (sent > 0)
    {
        // cool logic
    }
}

// usando long long pra (tentar) sobreviver ao ano 2038
long long Connection::timestamp(void)
{
    struct timeval tp;
    gettimeofday(&tp, NULL);
    return tp.tv_sec * 1000 + tp.tv_usec / 1000;
}
