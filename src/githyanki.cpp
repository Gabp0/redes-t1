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
#include "common.h"

using namespace std;
using namespace common;

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
    // memcpy(&buffer[size], &this->checksum, 1);

    return size;
}

void Githyanki::Frame::fromBytes(void *bytes)
{
    if (bytes)
    {
        char *msg = (char *)bytes;

        type = msg[0] >> 4;
        seq = msg[0] & 0xF;
        sizeData = msg[1];
        data = new char[sizeData];
        memcpy(data, &msg[3], this->sizeData);
        // memcpy(&this->checksum, &msg[this->sizeData + 4], 1);
    }
}

Githyanki::Frame::~Frame()
{
    if (data != NULL)
    {
        delete data;
        data = NULL;
    }
}

Githyanki::Frame::Frame(const char *data, size_t sizeData, unsigned short type, unsigned short seq)
{
    this->type = type;
    this->seq = seq;
    this->sizeData = sizeData;
    this->data = new char[sizeData + 1];

    memset(this->data, 0, sizeData + 1);
    memcpy(this->data, data, this->sizeData);

    // cout << data << endl;
    // cout << this->data << endl;
    // Arrumar para o checksum fazer do frame inteiro
    // char * buffer;
    // strtol(calcCheckSum(data).c_str(), &buffer, 2);
    // strncpy(this->checksum, buffer, 1);
    checksum[0] = 'a';
}

Githyanki::Frame::Frame(unsigned short type, unsigned short seq)
{
    // mark = Githyanki::SOH;
    this->type = type;
    this->seq = seq;
    data = new char[14];
    this->sizeData = 14;
    memset(this->data, 0, 14);

    // Arrumar para o checksum fazer do frame inteiro
    // memcpy(this->checksum, calcCheckSum(data).c_str(), 8);
}

Githyanki::Frame::Frame()
{
    data = NULL;
}

string Githyanki::Frame::toString()
{
    //"Mark: " + to_string(mark) +
    return "\nType: " + to_string(type) + " Seq: " + to_string(seq) + " DataSize: " + to_string(sizeData) + " Data: " + data + " Checksum: " + checksum;
}

int Githyanki::isValid(char *buffer, int tamanho_buffer, Frame *frame)
{
    int valid = 0;

    if (tamanho_buffer > 16)
        frame->fromBytes(buffer);

    for (int i = 0; i < (int)sizeof(VALID_TYPES); i++)
        if (frame->type == VALID_TYPES[i])
            valid = 1;

    if (!valid)
        return 0;
    // Check frame

    return 1;
}

// DataObject (Struct)
Githyanki::DataObject::DataObject()
{
    this->type = 0;
    this->name = NULL;
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
    this->name = NULL;
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
void Githyanki::Window::acknowledge(Githyanki::Ack *ack)
{
    // cout << "AhAck: " << ack->type << " Seq: " << ack->seq << endl;
    // New frames
    int buffer[Githyanki::SEND_WINDOW_MAX];
    int iBuffer = 0;
    int j = 0; // Quantity of not acked frames

    if (ack->type == Githyanki::ACK)
    {
        ack->seq++;
    }

    // cout << ack->seq << endl;

    for (int i = 0; i < Githyanki::SEND_WINDOW_MAX; i++)
    {
        if (window[i] < ack->seq)
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
    int i;
    for (i = 0; i < Githyanki::SEND_WINDOW_MAX; i++)
    {
        window[i] = i;
    }
    lastSeq = i - 1;
    firstNotFramedIndex = 0;
}

// Window Stuff
int Githyanki::Window::prepareFrames(Githyanki::DataObject *obj)
{
    char dataBuffer[Githyanki::DATA_SIZE_MAX];
    int qtyPrepared = 0;
    int i = firstNotFramedIndex;
    int seq, bytesToSend = 0;

    for (; i < Githyanki::SEND_WINDOW_MAX; i++)
    {
        memset(dataBuffer, 0, Githyanki::DATA_SIZE_MAX);
        seq = window[firstNotFramedIndex];
        bytesToSend = obj->size - obj->bytesFramed;

        if (bytesToSend == 0)
        {
            if (obj->type == Githyanki::TEXT)
                frames[i] = new Frame(Githyanki::END, seq);
            else
                frames[i] = new Frame(obj->name, obj->nameSize, Githyanki::END, seq);
            finishSeq = seq;
            firstNotFramedIndex++;
            qtyPrepared++;
            return qtyPrepared; // Acabou
        }
        if (bytesToSend > Githyanki::DATA_SIZE_MAX)
            bytesToSend = Githyanki::DATA_SIZE_MAX;

        memcpy(dataBuffer, obj->data + obj->bytesFramed, bytesToSend);
        obj->bytesFramed += bytesToSend;

        // cout << "bytes to send" << bytesToSend << endl
        //      << endl;
        // cout << dataBuffer << endl
        //      << endl;
        frames[i] = new Frame(dataBuffer, bytesToSend, obj->type, seq);
        // cout << frames[i]->toString() << endl;
        firstNotFramedIndex++;
        qtyPrepared++;
    }
    return qtyPrepared;
}

// Send first n frames from *frames
void sendNFrames(int n, Githyanki::Frame **frames, Connection *con)
{
    for (int i = 0; i < n; i++)
        con->sendFrame(frames[i]);
}

// int Githyanki::establishConnection(Connection *con)
// {
//     Frame initFrame = Frame(Githyanki::INIT, 0);
//     con->sendFrame(&initFrame);
//     Ack *ack = con->waitAcknowledge();
//     if (ack->type == Githyanki::ACK && ack->seq == 0)
//         return 1; // Ack
//     // Timeout
//     return 0;
// }

void flushBuffer(string **buffer, int size)
{
    lout << endl
         << "Flushing Data" << endl
         << endl;
    for (int i = 0; i < size; i++)
    {
        cout << buffer[i]->data();
        delete buffer[i];
    }
    cout << endl;
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
    finishedAcked = 0;
    finishedSeq = -2;
    windowDataSize = 0;
    lastAck = -1;

    obj = new DataObject();
    obj->bytesFramed = 0;
    obj->data = new char[1024];
    obj->size = 0;
}

void Githyanki::WindowRec::finish(Frame *frame)
{
    finished = true;
    if (obj->type == Githyanki::MEDIA)
    {
        char *name = new char[Githyanki::DATA_SIZE_MAX];
        memcpy(name, frame->data, frame->sizeData);
        obj->name = name;
        obj->nameSize = frame->sizeData;
    }
}

int distWindow(int f, int l, int s)
{
    if (f < l)
        return s - 1 - l + f;
    return f - l - 1;
}

bool ackIf(int f, int l, int s, int c)
{
    if (((f < l) && (s - 1 - l + f >= c)) || (((f > l)) && (f - l - 1 >= c)))
    {
        return 1;
    }
    return 0;
}

void Githyanki::WindowRec::bufferFrame(Frame *frame)
{
    if (frame->type == Githyanki::TIMEOUT)
    {
        obj->otherCon->acknowledge(firstSeq, 1);
        lastAck = (firstSeq + 15) % 16;
        return;
    }

    if (obj->type == 0)
        obj->type = frame->type;

    if (!(frame->type == Githyanki::TEXT) && !(frame->type == Githyanki::MEDIA) && !(frame->type == Githyanki::END))
        return;

    int windowSeqIndex;
    for (int i = 0; i < Githyanki::SEND_WINDOW_MAX; i++)
    {
        if (windowPlace[i].seq == frame->seq)
        {
            windowSeqIndex = i;
            break;
        }
        if (windowPlace[i].seq > frame->seq)
            return;
    }

    if (frame->type == Githyanki::END)
    {
        lout << "Finish frame received\n\tSeq - " << frame->seq << endl;

        finishedSeq = frame->seq;
        finish(frame);
    }
    else
    {
        lout << "Data frame sended:\n\tType - " << (frame->type == Githyanki::TEXT ? "Text" : "Media") << "\n\tSeq - " << frame->seq << endl;

        string *data = new string(frame->data);
        windowData[windowPlace->posi % 256] = data;
        windowDataSize++;
        lastDataIndex++;
        lastSeq = (lastSeq + 1) % Githyanki::WINDOW_MAX;
    }

    for (int i = windowSeqIndex; i < Githyanki::SEND_WINDOW_MAX - 1; i++)
        windowPlace[i] = windowPlace[i + 1];

    windowPlace[Githyanki::SEND_WINDOW_MAX - 1] = place(lastSeq, lastDataIndex);
    firstSeq = windowPlace[0].seq;

    if (ackIf(firstSeq, lastAck, Githyanki::WINDOW_MAX, Githyanki::SEND_WINDOW_MAX))
    {
        lastAck = (lastAck + 8) % 16;
        obj->otherCon->acknowledge(lastAck);
    }
    // End frame received, trying to finish download
    if (finishedSeq > -1)
    {
        int distFinish = distWindow(finishedSeq + 1, lastAck, Githyanki::WINDOW_MAX);
        if (ackIf(firstSeq, lastAck, Githyanki::WINDOW_MAX, distFinish))
        {
            lout << "Acking end frame" << endl;
            obj->otherCon->acknowledge(finishedSeq);
            lastAck = finishedSeq;
        }
        else
        {
            obj->otherCon->acknowledge(firstSeq, 1);
            lastAck = firstSeq;
        }
    }

    delete frame;

    if (windowDataSize >= 255)
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

    while (window.finishedSeq != window.lastAck)
    {
        frame = new Frame();
        frame = myCon->receiveFrame();
        window.bufferFrame(frame);
    }

    flushBuffer(window.windowData, window.windowDataSize);
    lout << "Finished" << endl;

    return window.obj;
}

int Githyanki::SlidingWindowSend(Githyanki::DataObject *obj)
{
    // Resolve Conflicts
    // To Do
    Connection *myCon = obj->myCon;
    Connection *otherCon = obj->otherCon;
    Ack *ack;
    Githyanki::Window window = {};

    window.init();
    int sendingFrames = 0;
    obj->frameQty = ceil(sizeof(obj->data) / Githyanki::DATA_SIZE_MAX);

    sendingFrames = window.prepareFrames(obj);

    while (sendingFrames > 0)
    {
        sendNFrames(sendingFrames, window.frames, otherCon);

        // Wait to recieve Ack or Nack
        ack = myCon->waitAcknowledge();

        if (ack->type != Githyanki::TIMEOUT)
        {
            if (window.finishSeq == ack->seq)
            {
                lout << "End frame acked" << endl;
                break;
            }
            window.acknowledge(ack);
            sendingFrames = window.prepareFrames(obj);
        }
    }
    lout << "Finished";

    return 1;
}