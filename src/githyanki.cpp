#include "githyanki.h"
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

int Githyanki::initSocket(string device)
{
    char *device_c = new char[device.length() + 1];
    strcpy(device_c, device.c_str());
    int socket = cria_raw_socket(device_c);
    cout << socket << endl;
    return socket;
}

// retorna -1 se deu timeout, ou quantidade de bytes lidos
int Githyanki::receiveMessage(int socket, int timeoutMillis, char *buffer, int tamanho_buffer)
{
    long long comeco = timestamp();
    struct timeval timeout = {.tv_sec = 0, .tv_usec = timeoutMillis * 1000};
    setsockopt(socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout));
    int bytes_lidos;
    do
    {
        bytes_lidos = recv(socket, buffer, tamanho_buffer, 0);
        if (Githyanki::isValid(buffer, bytes_lidos))
        {
            return bytes_lidos;
        }
    } while (timestamp() - comeco <= timeoutMillis);
    return -1;
}

void *Githyanki::frame::toBytes()
{
    size_t size = this->size + 6;
    char *bytes = new char[size];

    unsigned int header = ((this->mark << 18) | (this->type << 12) | (this->seq << 8) | (this->size));

    memcpy(&bytes[0], &header, 4);
    cout << *bytes << endl;
    memcpy(&bytes[3], this->data, this->size);
    bytes[size - 1] = this->checksum;

    cout << bytes << endl;

    return bytes;
}

void Githyanki::sendMessage(frame msg, int socket)
{
    void *bytes = msg.toBytes();
    size_t size = msg.size + 6;
    ssize_t sent = send(socket, bytes, size, 0);

    if (sent > 0)
    {
        // cool logic
    }
}

Githyanki::frame *Githyanki::createFrame(const char *data, size_t data_size, unsigned short type, unsigned short seq)
{
    Githyanki::frame *f = new Githyanki::frame;
    f->mark = Githyanki::SOH;
    f->type = type;
    f->seq = seq;
    f->size = data_size;
    memcpy(f->data, data, data_size);
    f->checksum = 0x12;

    return f;
}

// usando long long pra (tentar) sobreviver ao ano 2038
long long Githyanki::timestamp()
{
    struct timeval tp;
    gettimeofday(&tp, NULL);
    return tp.tv_sec * 1000 + tp.tv_usec / 1000;
}

int Githyanki::isValid(char *buffer, int tamanho_buffer)
{
    if (tamanho_buffer <= 0)
    {
        return 0;
    }
    // insira a sua validação de Githyankio aqui
    // return buffer == "0x7f"; // Esta errado
    return 1;
}
