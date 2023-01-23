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
#include <iomanip>

using namespace std;

void *Githyanki::frame::toBytes()
{
    size_t size = this->size + 5;
    char *bytes = new char[size];

    char header[3];
    header[0] = ((this->mark << 2) | (this->type >> 6));
    header[1] = ((this->type << 4) | (this->seq));
    header[2] = this->size;

    memcpy(bytes, &header, sizeof(header));
    memcpy(&bytes[3], this->data, this->size);
    memcpy(&bytes[size - 1], &this->checksum, 2);

    // print bytes as hex
    for (size_t i = 0; i < size; ++i)
        std::cout << std::setfill('0') << std::setw(2) << std::hex << (0xff & (unsigned int)bytes[i]);
    cout << endl;

    return bytes;
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

int Githyanki::isValid(char *buffer, int tamanho_buffer)
{
    if (tamanho_buffer <= 0)
    {
        return 0;
    }

    // cool validation

    return 1;
}
