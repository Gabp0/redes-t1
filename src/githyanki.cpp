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
#include <vector>

using namespace std;

void *Githyanki::frame::toBytes()
{
    size_t size = this->size + 6;
    char *bytes = new char[size];

    uint32_t header = ((this->mark << 18) | (this->type << 12) | (this->seq << 8) | (this->size));
    cout << header << endl;

    memcpy(&bytes[0], &header, 4);
    memcpy(&bytes[3], this->data, this->size);
    memcpy(&bytes[size - 2], &this->checksum, 2);

    for (size_t i = 0; i < size; i++)
    {
        int c = bytes[i];
        cout << c;
    }
    cout << endl;

    vector<bool> out();

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
