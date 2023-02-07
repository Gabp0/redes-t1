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
#include "connection.h"

using namespace std;

// Frame Stuff
size_t Githyanki::Frame::toBytes(char *buffer)
{
    size_t size = sizeData + 3;

    char headerA = type << 4;
    headerA = headerA | seq;
    char headerB = sizeData;

    buffer[0] = headerA;
    buffer[1] = headerB;

    memcpy(&buffer[3], this->data, this->sizeData);
    memcpy(&buffer[size - 1], &this->checksum, 1);

    return size;
}

void Githyanki::Frame::fromBytes(void *bytes)
{
    char *msg = (char *)bytes;

    this->type = msg[0] >> 4;
    this->seq = msg[0] & 0xF;
    this->sizeData = msg[1];
    memcpy(&this->data, &msg[3], this->sizeData);
    memcpy(&this->checksum, &msg[this->sizeData + 4], 2);
}

Githyanki::Frame::Frame(const char *data, size_t data_size, unsigned short type, unsigned short seq)
{
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

    // Check frame

    return 1;
}

// DataObject (Struct)
Githyanki::DataObject::DataObject()
{
    this->type = 0;
}

Githyanki::DataObject::~DataObject()
{
    if (name != NULL)
    {
        delete (name);
    }
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
    if (sizeof(name) > Githyanki::DATA_SIZE_MAX)
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
    // New frames
    int buffer[Githyanki::SEND_WINDOW_MAX];
    int iBuffer = 0;
    int j = 0; // Quantity of not acked frames

    if (ack.type == Githyanki::ACK)
    {
        ack.seq++;
    }

    for (int i = 0; i < (int)sizeof(this->window); i++)
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
    for (int i = 0; i < Githyanki::SEND_WINDOW_MAX; i++)
    {
        window[i] = i;
    }
    lastSeq = 6;
    firstNotFramedIndex = 0;
}

// Window Stuff
int prepareFrames(Githyanki::Window *win, Githyanki::DataObject *obj)
{
    char dataBuffer[Githyanki::DATA_SIZE_MAX];
    int qtyPrepared = 0;
    int i = win->firstNotFramedIndex;
    int seq, bytesToSend = 0;

    for (; i < Githyanki::SEND_WINDOW_MAX; i++)
    {
        seq = win->window[win->firstNotFramedIndex];
        bytesToSend = obj->size - obj->bytesFramed;

        if (bytesToSend == 0)
            return qtyPrepared; // Acabou
        if (bytesToSend > Githyanki::DATA_SIZE_MAX)
            bytesToSend = Githyanki::DATA_SIZE_MAX;

        memcpy(dataBuffer, obj->data + obj->bytesFramed, bytesToSend);
        obj->bytesFramed += bytesToSend;

        win->frames[i] = new Githyanki::Frame(dataBuffer, bytesToSend, obj->type, seq);
        win->firstNotFramedIndex++;
        qtyPrepared++;
    }
    return qtyPrepared;
}

// Send first n frames from *frames
void sendNFrames(int n, Githyanki::Frame **frames, Connection *con)
{
    for (int i = 0; i < n; i++)
    {
        con->sendFrame(frames[i]);
    }
}

// End frame without name
void sendEndFrame(int seq, Connection *con)
{
    Githyanki::Frame endFrame = Githyanki::Frame(Githyanki::END, seq);
    con->sendFrame(&endFrame);
}

// End frame with name
void sendEndFrame(int seq, Connection *con, char *name, int size)
{
    Githyanki::Frame endFrame = Githyanki::Frame(name, size, Githyanki::END, seq);
    con->sendFrame(&endFrame);
}

int Githyanki::establishConnection(Connection *con)
{
    Frame initFrame = Frame(Githyanki::INIT, 0);
    con->sendFrame(&initFrame);
    Ack ack = con->waitAcknowledge();
    if (ack.type == Githyanki::ACK && ack.seq == 0)
        return 1; // Ack
    // Timeout
    return 0;
}

int Githyanki::SlidingWindowSend(Githyanki::DataObject *obj)
{
    // Resolve Conflicts
    // To Do
    Connection *myCon = obj->myCon;
    Connection *otherCon = obj->otherCon;
    Ack ack;
    Githyanki::Window window = {};

    window.init();
    int sendingFrames = 0;
    obj->frameQty = ceil(sizeof(obj->data) / Githyanki::DATA_SIZE_MAX);

    sendingFrames = prepareFrames(&window, obj);
    // cout << sendingFrames << endl;
    // for (Frame *f : window.frames)
    // {
    //     cout << f->toString() << endl
    //          << endl;
    // }

    while (sendingFrames > 0)
    {
        sendNFrames(sendingFrames, window.frames, otherCon);

        // Wait to recieve Ack or Nack
        ack = myCon->waitAcknowledge();

        if (ack.type != Githyanki::TIMEOUT)
        {
            window.acknowledge(ack);
            sendingFrames = prepareFrames(&window, obj);
        }
    }

    if (obj->type == Githyanki::TEXT)
        sendEndFrame(window.lastSeq, otherCon);
    else
        sendEndFrame(window.lastSeq, otherCon, obj->name, obj->nameSize);

    return 1;
}

void flushBuffer(Frame **buffer, int size)
{
    for (int i = 0; i < size; i++)
    {
        cout << buffer[i]->data;
        delete buffer[i];
    }
}

void flushBuffer(string **buffer, int size)
{
    for (int i = 0; i < size; i++)
    {
        cout << buffer[i];
        delete buffer[i];
    }
}

void Githyanki::WindowRec::init()
{
    int i = 0;
    for (; i < Githyanki::SEND_WINDOW_MAX; i++)
    {
        windowPlace[i] = place(i, i);
    }
    lastSeq = i - 1;
    lastDataIndex = i - 1;
    firstSeq = 0;
    finished = 0;
    windowDataSize = 0;
    notAck = 0;
    lastAck = -1;

    obj = new DataObject();
    obj->bytesFramed = 0;
    obj->data = new char[1024];
    obj->size = 0;
}

void Githyanki::WindowRec::finish(Frame *frame)
{
    finished = !finished;
    if (obj->type == Githyanki::MEDIA)
    {
        char *name = new char[Githyanki::DATA_SIZE_MAX];
        memcpy(name, frame->data, frame->sizeData);
        obj->name = name;
        obj->nameSize = frame->sizeData;
    }
}

void Githyanki::WindowRec::bufferFrame(Frame *frame)
{
    if (frame->type != Githyanki::TIMEOUT){
        obj->otherCon->acknowledge(firstSeq, 1);
        lastAck = (firstSeq + 15) % 16;
        return;
    }

    if (frame->type == Githyanki::END)
    {
        finish(frame);
        return;
    }

    if (obj->type == 0)
        obj->type = frame->type;

    if(!(frame->seq >= firstSeq) || !(frame->seq <= lastSeq))
        return;
    
    int windowSeqIndex;
    for(int i = 0; i < Githyanki::SEND_WINDOW_MAX; i++){
        if(windowPlace[i].seq == frame->seq){
            windowSeqIndex = i;
            break;
        } 
        if(windowPlace[i].seq > frame->seq)
        return;
    }
    
    string *data = new string(frame->data);
    windowData[windowPlace->posi % 256] = data;
    windowDataSize++;

    lastDataIndex++;
    lastSeq = (lastSeq + 1) % Githyanki::WINDOW_MAX;

    for(int i = windowSeqIndex; Githyanki::SEND_WINDOW_MAX - 1; i++){
        windowPlace[windowSeqIndex] = windowPlace[windowSeqIndex+1];
    }

    windowPlace[Githyanki::SEND_WINDOW_MAX - 1] = place(lastSeq, lastDataIndex);
    firstSeq = windowPlace[0].seq;
    
    if(firstSeq - lastAck >= Githyanki::SEND_WINDOW_MAX/2){
        obj->otherCon->acknowledge(frame->seq);
        lastAck = frame->seq;
    }

    delete frame;
    
    if (windowDataSize >= 7)
    {
        flushBuffer(windowData, windowDataSize);
        windowDataSize = 0;        
    }
}

Githyanki::DataObject *Githyanki::SlidingWindowReceive(Connection *myCon, Connection *otherCon)
{
    WindowRec window = {};
    window.init();
    window.obj->myCon = myCon;
    window.obj->otherCon = otherCon;

    Frame *frame;

    while (!window.finished)
    {
        cout << "hello" << endl;
        frame = new Frame();
        frame = myCon->receiveFrame();
        cout << frame->toString() << endl;

        //window.bufferFrame(frame);
        
    }
    
    flushBuffer(window.windowData, window.windowDataSize);

    return window.obj;
}
