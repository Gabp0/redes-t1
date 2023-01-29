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
#include <string>
#include <netinet/in.h>
#include <iomanip>
#include <iostream>
#include <cmath>

using namespace std;

// Frame Stuff
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

int Githyanki::isValid(char *buffer, int tamanho_buffer, Frame *frame)
{
    frame->fromBytes(buffer);

    //Check frame

    return 1;
}

// DataObject (Struct)
Githyanki::DataObject::DataObject(){}

Githyanki::DataObject::~DataObject(){
    delete(name);
}

Githyanki::DataObject::DataObject(char *data)
{
    this->data = data;
    this->bytesFramed = 0;
    this->size = sizeof(this->data);
}

Githyanki::DataObject::DataObject(char *data, char *name)
{
    this->data = data;
    this->bytesFramed = 0;
    this->size = sizeof(this->data);
    this->name = name;
    if(sizeof(name) > Githyanki::DATA_SIZE_MAX)
    this->nameSize = Githyanki::DATA_SIZE_MAX;
    else
    this->nameSize = sizeof(name);

}

// Ack (struct)

// Githyanki::Ack::Ack(){

// }

// Window (Struct)
void Githyanki::Window::acknowledge(Githyanki::Ack ack)
{
    //New frames
    int buffer[Githyanki::SEND_WINDOW_MAX];
    int iBuffer = 0;
    int j = 0; //Quantity of not acked frames

    if (ack.type == Githyanki::AWK)
    {
        ack.seq++;
    }

    for (int i = 0; i < sizeof(this->window); i++)
    {
        if (window[i] < ack.seq)
        {
            lastSeq = (lastSeq + 1) % Githyanki::WINDOW_MAX;
            buffer[iBuffer] = lastSeq;
            delete (frames[i]);
            iBuffer++;
        }
        else
        {
            window[j] = window[i];
            frames[j] = frames[i];
            j++;
        }
    }

    for (int i = 0; i < iBuffer; i++)
    {
        window[j + i] = buffer[i];
    }
    firstNotFramedIndex = j;
}

void Githyanki::Window::init()
{
    for (int i = 0; i < sizeof(this->window); i++)
    {
        window[i] = i;
    }
    lastSeq = 6;
    firstNotFramedIndex = 0;
}

// Window Stuff

int prepareFrames(Githyanki::Window *win, Githyanki::DataObject *obj)
{
    int qtyPrepared = 0;
    int i = win->firstNotFramedIndex;
    for (; i < Githyanki::SEND_WINDOW_MAX; i++)
    {
        int seq = win->window[win->firstNotFramedIndex];
        char dataBuffer[Githyanki::DATA_SIZE_MAX];
        int bytesToSend = obj->size - obj->bytesFramed;
        if (bytesToSend == 0)
            return qtyPrepared; // Acabou
        if (bytesToSend > Githyanki::DATA_SIZE_MAX)
            bytesToSend = Githyanki::DATA_SIZE_MAX;

        memcpy(dataBuffer, obj->data + obj->bytesFramed, bytesToSend);

        win->frames[i] = new Githyanki::Frame(dataBuffer, bytesToSend, obj->type, seq);
        win->firstNotFramedIndex++;
        qtyPrepared++;
    }
    return qtyPrepared;
}

// Send first n frames from *frames
int sendNFrames(int n, Githyanki::Frame **frames, Connection *con)
{
    for (int i = 0; i < n; i++)
    {
        con->sendFrame(frames[i]);
    }
}

//End frame without name
int sendEndFrame(int seq)
{
    Githyanki::Frame endFrame = Githyanki::Frame(Githyanki::END, seq);
}

//End frame with name
int sendEndFrame(int seq, char *name, int size)
{
    Githyanki::Frame endFrame = Githyanki::Frame(name, size, Githyanki::END, seq);
}

int Githyanki::SlidingWindowSend(Githyanki::DataObject *obj)
{
    // Resolve Conflicts
    // To Do
    Githyanki::Frame frames[6];
    Connection *con = obj->con;
    Ack ack;
    int nextSeq = 0;
    int sendingFrames = 0;
    obj->frameQty = ceil(sizeof(obj->data) / Githyanki::DATA_SIZE_MAX);

    Githyanki::Window window = {};
    window.init();

    sendingFrames = prepareFrames(&window, obj);

    while (sendingFrames <= 0)
    {
        sendNFrames(sendingFrames, window.frames, con);

        // Wait to recieve Ack or Nack
        ack = con->waitAcknowledge();

        if (ack.type != Githyanki::TIMEOUT)
        {
            window.acknowledge(ack);
            sendingFrames = prepareFrames(&window, obj);
        }
    }

    if(obj->type == Githyanki::TEXT)
    sendEndFrame(window.lastSeq);
    else
    sendEndFrame(window.lastSeq, obj->name, obj->nameSize);

    return 1;
}

Githyanki::DataObject Githyanki::SlidingWindowReceive(Connection con)
{
    DataObject obj = DataObject();
    Githyanki::Frame *bufferRecieved[256];
    bool finished = 0;
    Frame frame;

    while (!finished)
    {
        frame = con.receiveFrame();
        if(frame.type == Githyanki::END){
            finished++;
            char *name = new char[Githyanki::DATA_SIZE_MAX];
            memcpy(name, frame.data, frame.sizeData);
            obj.name = name;
            obj.nameSize = frame.sizeData;
        }


    }

    return obj;
}
