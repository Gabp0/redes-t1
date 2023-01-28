#include "githyanki.h"
#include "sockets/socket.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
//#include <sys/socket.h>
//#include <sys/time.h>
//#include <unistd.h>
#include <cstring>
#include <string>
//#include <netinet/in.h>
#include <iomanip>
#include <iostream>
#include "checksum.h"

using namespace std;

size_t Githyanki::Frame::toBytes(char *buffer)
{
    size_t size = sizeData + 3;

    bitset<6> bsetType(type);
    bitset<4> bsetSeq(seq);
    bitset<6> bsetSize(sizeData);
    cout << bsetType << endl;
    cout << bsetSeq << endl;
    cout << bsetSize << endl;

    bitset<16> bsetHeader(bsetType.to_string() + bsetSeq.to_string() + bsetSize.to_string());
    cout << bsetHeader << endl;

    cout << endl;
    char headerA = type << 2;
    headerA = headerA | seq >> 2;
    char headerB = seq << 6;
    headerB = headerB | sizeData;
    buffer[0] = headerA;
    buffer[1] = headerB;
    memcpy(&buffer[3], this->data, this->sizeData);
    memcpy(&buffer[size - 1], &this->checksum, 1);

    return size;
}

void Githyanki::Frame::fromBytes(void *bytes)
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
    this->seq = seq | ((msg[1] & 0xc0) >> 6);
    this->sizeData = msg[1] & 0x3f;
    memcpy(&this->data, &msg[3], this->sizeData);
    memcpy(&this->checksum, &msg[this->sizeData + 4], 2);
}

Githyanki::Frame::Frame(const char *data, size_t data_size, unsigned short type, unsigned short seq)
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

string Githyanki::Frame::toString()
{
    //"Mark: " + to_string(mark) +
    return "Frame\nFrame toString\nType: " + to_string(type) + " Seq: " + to_string(seq) + " DataSize: " + to_string(sizeData) + " Data: " + data + " Checksum: " + checksum;
}



int prepareFrame(int seq, Githyanki::SendObject *obj){


}

//Send first n frames from *frames
int sendNFrames(int n, Githyanki::Frame *frames, Connection *con){
    for(int i = 0; i < n;i++){
        con->sendMessage(frames[i]);
    }
}

int Githyanki::SlidingWindow(Githyanki::SendObject *obj){
    //Resolve Conflicts
    //To Do
    Githyanki::Frame frames[6];
    Connection *con = obj->con;
    int awc = 0;
    int nextSeq = 0;
    int sentFrames = 0;
    int sendingFrames = 0;
    obj->frameQty = ceil(sizeof(obj->data) / Githyanki::DATA_SIZE_MAX);


    while(sentFrames < obj->frameQty){
        sendingFrames = prepareFrame(nextSeq, obj);
        sendNFrames(sendingFrames, frames, con);
        
        awc = con->waitAcknowledge();
        if(awc == -2){
            awc = con->waitAcknowledge();
        }
        if(awc >= 0){
            nextSeq = (awc+1)%Githyanki::SEQUENCE_MAX;
            sentFrames += awc;
        }
    }

    return 1;
}

Githyanki::Frame::Frame(unsigned short type, unsigned short seq)
{
    // mark = Githyanki::SOH;
    this->type = type;
    this->seq = seq;
    this->sizeData = 0;
    // Arrumar para o checksum fazer do frame inteiro
    // memcpy(this->checksum, calcCheckSum(data).c_str(), 8);
}

Githyanki::Frame::Frame() {}

Githyanki::SendObject::SendObject() {
    this->sended = 0;
}

void Githyanki::printFrame(Githyanki::Frame *f)
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
