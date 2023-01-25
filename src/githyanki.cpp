#include "githyanki.h"
#include "sockets/socket.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>
// #include <cstring>
#include <string>
#include <netinet/in.h>
#include <iomanip>
#include <iostream>
#include "checksum.h"

using namespace std;

size_t Githyanki::frame::toBytes(char *buffer)
{
    size_t size = sizeData + 3;
    // string sBuffer;

    // cout << endl
    //      << "toBytes" << endl;

    bitset<6> bsetType(type);
    bitset<4> bsetSeq(seq);
    bitset<6> bsetSize(sizeData);
    cout << bsetType << endl;
    cout << bsetSeq << endl;
    cout << bsetSize << endl;

    bitset<16> bsetHeader(bsetType.to_string() + bsetSeq.to_string() + bsetSize.to_string());
    cout << bsetHeader << endl;
    // unsigned long lHeader = bsetHeader.to_ulong();

    cout << endl;
    char headerA = type << 2;
    headerA = headerA | seq >> 2;
    char headerB = seq << 6;
    headerB = headerB | sizeData;
    printf("A: %02x, B: %02x\n", headerA, headerB);
    buffer[0] = headerA;
    buffer[1] = headerB;
    memcpy(&buffer[3], this->data, this->sizeData);
    memcpy(&buffer[size - 1], &this->checksum, 1);

    // print bytes as hex
    // for (size_t i = 0; i < size; ++i)
    //    std::cout << std::setfill('0') << std::setw(2) << std::hex << (0xff & (unsigned int)bytes[i]);
    // cout << endl;

    return size;
}

void Githyanki::frame::fromBytes(void *bytes)
{
    char *msg = (char *)bytes;
    cout << "Frame: ";
    for (size_t i = 0; i < sizeof(msg); i++)
    {
        printf("%02x", msg[i]);
    }
    cout << endl;

    this->type = msg[0] >> 2;
    this->seq = (msg[0] & 0x3) << 2;
    this->seq = seq | ((msg[1]& 0xc0) >> 6);
    this->sizeData = msg[1] & 0x3f;
    memcpy(&this->data, &msg[3], this->sizeData);
    memcpy(&this->checksum, &msg[this->sizeData + 4], 2);
}

Githyanki::frame::frame(const char *data, size_t data_size, unsigned short type, unsigned short seq)
{
    // mark = Githyanki::SOH;
    this->type = type;
    this->seq = seq;
    this->sizeData = data_size;
    memcpy(this->data, data, data_size);
    // Arrumar para o checksum fazer do frame inteiro
    // char * buffer;
    // strtol(calcCheckSum(data).c_str(), &buffer, 2);
    // strncpy(this->checksum, buffer, 1);
    checksum[0] = 'a';
}

string Githyanki::frame::toString()
{
    //"Mark: " + to_string(mark) +
    return "Frame\nFrame toString\nType: " + to_string(type) + " Seq: " + to_string(seq) + " DataSize: " + to_string(sizeData) + " Data: " + data + " Checksum: " + checksum;
}

Githyanki::frame::frame(unsigned short type, unsigned short seq)
{
    // mark = Githyanki::SOH;
    this->type = type;
    this->seq = seq;
    this->sizeData = 0;
    // Arrumar para o checksum fazer do frame inteiro
    // memcpy(this->checksum, calcCheckSum(data).c_str(), 8);
}

Githyanki::frame::frame() {}

void Githyanki::printFrame(Githyanki::frame *f)
{
    unsigned char *charPtr = (unsigned char *)f;
    cout << "Print Frame: ";
    for (size_t i = 0; i < sizeof(f); i++)
    {
        printf("%02x", charPtr[i]);
    }
    cout << endl;
}

int Githyanki::isValid(char *buffer, int tamanho_buffer)
{
    if (tamanho_buffer <= 0)
    {
        return 0;
    }

    return 1;
}
