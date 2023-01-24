#include "githyanki.h"
#include "sockets/socket.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>
//#include <cstring>
#include <string>
#include <netinet/in.h>
#include <iomanip>
#include <iostream>
#include "checksum.h"

using namespace std;

size_t Githyanki::frame::toBytes(char *buffer)
{
    size_t size = this->dataSize + 5;

    char header[3];
    header[0] = ((this->mark << 2) | (this->type >> 6));
    header[1] = ((this->type << 4) | (this->seq));
    header[2] = this->dataSize;

    memcpy(buffer, &header, sizeof(header));
    memcpy(&buffer[3], this->data, this->dataSize);
    memcpy(&buffer[size - 1], &this->checksum, 2);

    // print bytes as hex
    // for (size_t i = 0; i < size; ++i)
    //    std::cout << std::setfill('0') << std::setw(2) << std::hex << (0xff & (unsigned int)bytes[i]);
    // cout << endl;

    return size;
}

void Githyanki::frame::fromBytes(void *bytes)
{
    char *msg = (char *)bytes;

    this->mark = (msg[0] >> 2);
    this->seq = (msg[0] << 6) | (msg[1] >> 4);
    memcpy(&this->dataSize, &msg[2], 1);
    memcpy(&this->data, &msg[3], this->dataSize);
    memcpy(&this->checksum, &msg[this->dataSize + 4], 2);

    cout << msg << endl;
    cout << this->mark << " " << this->seq << endl;
    cout << this->data << endl;
}

Githyanki::frame::frame(const char *data, size_t data_size, unsigned short type, unsigned short seq)
{
    mark = Githyanki::SOH;
    this->type = type;
    this->seq = seq;
    this->dataSize = data_size;
    memcpy(this->data, data, data_size);
    //Arrumar para o checksum fazer do frame inteiro
    memcpy(this->checksum, calcCheckSum(data).c_str(), 8);
    cout << this->checksum << endl;
}

string Githyanki::frame::toString()
{
    return "Type: " + to_string(type) + " Seq: " + to_string(seq) + " Data: " + data + " Checksum: " + checksum;
}

Githyanki::frame::frame(unsigned short type, unsigned short seq)
{
    mark = Githyanki::SOH;
    this->type = type;
    this->seq = seq;
    this->dataSize = 0;
    //Arrumar para o checksum fazer do frame inteiro
    //memcpy(this->checksum, calcCheckSum(data).c_str(), 8);
}

Githyanki::frame::frame(){}

int Githyanki::isValid(char *buffer, int tamanho_buffer)
{
    if (tamanho_buffer <= 0)
    {
        return 0;
    }

    // cool validation

    return 1;
}
