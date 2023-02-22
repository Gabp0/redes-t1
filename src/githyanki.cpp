#include "githyanki.h"
#include "sockets/socket.h"
#include "error_handling/errors.h"
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

int Githyanki::isValid(char *buffer, int tamanho_buffer, Frame *frame)
{
    int valid = 0;

    if (tamanho_buffer > 16)
        frame->fromBytes(buffer);

    for (int i = 0; i < (int)sizeof(VALID_TYPES); i++)
        if (frame->type == VALID_TYPES[i])
            valid = 1;

    if (frame->data == NULL)
        return 0;

    // Check frame

    return valid;
}

// Frame Stuff
size_t Githyanki::Frame::toBytes(char *buffer)
{
    size_t size = sizeData + 3;
    int offset = 0;

    char headerA = type << 4;
    headerA = headerA | seq;
    char headerB = sizeData;

    buffer[0] = headerA;
    buffer[1] = headerB;

    memcpy(&buffer[3], this->data, this->sizeData);
    if (size < 16)
    {
        offset = 17 - size;
        memset(&buffer[3 + this->sizeData], 0, offset);
        size += offset;
    }
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
    safe_delete(data);
}

Githyanki::Frame::Frame(const char *data, size_t sizeData, unsigned short type, unsigned short seq)
{
    this->type = type;
    this->seq = seq;
    this->sizeData = sizeData;
    this->data = new char[sizeData + 1];

    memset(this->data, 0, sizeData + 1);
    memcpy(this->data, data, this->sizeData);

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

void Githyanki::Frame::toString()
{
    if (this->data == NULL)
    {
        // lout << "NULL Frame" << endl;
        // lout << "Type: " << type << " Seq: " << seq << " DataSize: " << sizeData;
        return;
    }
    DataBlock data = DataBlock(this->data, sizeData);
    // lout << "Type: " << type << " Seq: " << seq << " DataSize: " << sizeData << " Data: " << data.data << " Checksum: " << checksum;
}

Githyanki::DataBlock::~DataBlock()
{
    safe_delete(data);
}

Githyanki::DataBlock::DataBlock(char *data, int size)
{
    this->data = new char[size + 1];
    this->size = size;
    memcpy(this->data, data, size);
    memcpy(this->data + size, "\0", 1);
}

// DataObject (Struct)
Githyanki::DataObject::DataObject()
{
    this->type = 0;
    this->name = NULL;
}

Githyanki::DataObject::~DataObject()
{
    safe_delete(name);
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

char *Githyanki::DataObject::getBytes(int size)
{
    char *data = new char[size];
    if (this->type == Githyanki::TEXT)
    {
        memcpy(data, this->data + this->bytesFramed, size);
    }
    else
    {
        fread(data, 1, size, this->file);
    }
    return data;
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

void Githyanki::WindowRec::init()
{
    int i = 0;
    for (; i < Githyanki::SEND_WINDOW_MAX; i++)
    {
        windowPlace[i] = new place(i, i);
    }
    lastSeq = i - 1;
    lastDataIndex = i - 1;
    firstSeq = 0;
    finishedAcked = 0;
    endSeq = -2;
    windowDataSize = 0;
    lastAck = -1;
    receivedFrames = 0;

    obj = new DataObject();
    obj->bytesFramed = 0;
    obj->data = new char[1024];
    obj->size = 0;
}

void Githyanki::WindowRec::finalize(Frame *frame)
{
    if (obj->type == Githyanki::FILE)
    {
        char *name = new char[frame->sizeData+1];
        memcpy(name, frame->data, frame->sizeData);
        memcpy(name+frame->sizeData, "\0", 1);
        obj->name = name;
        obj->nameSize = frame->sizeData;
    }
}

void Githyanki::WindowRec::acknowledge()
{
    Ack *ack = NULL;

    // ACK
    if (receivedFrames == Githyanki::SEND_WINDOW_MAX)
    {
        lastAck = (lastAck + Githyanki::SEND_WINDOW_MAX) % Githyanki::WINDOW_MAX;
        ack = new Ack(Githyanki::ACK, lastAck);
    }

    // Find best ack, nack or ack
    // Find the seq for NACK
    for (place *p : windowPlace)
    {
        if (!p->received)
        {
            lastAck = (p->seq + Githyanki::WINDOW_MAX - 1) % Githyanki::WINDOW_MAX;
            ack = new Ack(Githyanki::NACK, p->seq);
            break;
        }
    }

    obj->otherCon->acknowledge(*ack);

    // New frames
    place *buffer[Githyanki::SEND_WINDOW_MAX];
    int iBuffer = 0; //
    int j;           // Quantity of not acked frames
    int i;           // Frames passed

    for (i = 0; i < Githyanki::SEND_WINDOW_MAX; i++)
    {
        // Break for Nack`s
        if (windowPlace[i]->seq == ack->seq && ack->type == Githyanki::NACK)
            break;

        // Ack frame and populate new places
        lastAck = windowPlace[i]->seq;
        lastSeq = (lastSeq + 1) % Githyanki::WINDOW_MAX;
        lastDataIndex = (lastDataIndex + 1) % 256;
        buffer[iBuffer] = new place(lastSeq, lastDataIndex);
        iBuffer++;
        receivedFrames--;

        // Break for Ack`s
        if (windowPlace[i]->seq == ack->seq && ack->type == Githyanki::ACK)
        {
            i++;
            safe_delete(windowPlace[i]);
            break;
        }

        safe_delete(windowPlace[i]);
    }

    // Deslocate not acked
    for (j = 0; i < Githyanki::SEND_WINDOW_MAX; i++, j++)
        windowPlace[j] = windowPlace[i];

    // Insert new places
    for (int i = 0; i < iBuffer; i++)
        windowPlace[j + i] = buffer[i];

    firstSeq = windowPlace[0]->seq;
    safe_delete(ack);
}

void Githyanki::WindowRec::bufferFrame(Frame *frame)
{
    // Rejecting frames
    // Sending Nack cause TIMEOUT
    if (frame->type == Githyanki::TIMEOUT)
    {
        acknowledge();
        safe_delete(frame);
        return;
    }

    // Reject non protocol frames
    if (!(frame->type == Githyanki::TEXT) && !(frame->type == Githyanki::FILE) && !(frame->type == Githyanki::END))
    {
        safe_delete(frame);
        return;
    }

    // Find seq in windowPlace
    int windowSeqIndex = -1;
    for (int i = 0; i < Githyanki::SEND_WINDOW_MAX; i++)
    {
        // Reject seq after end frame
        if (windowPlace[i]->seq == endSeq)
        {
            break;
        }
        if (windowPlace[i]->seq == frame->seq && !(windowPlace[i]->received))
        {
            windowSeqIndex = i;
            break;
        }
    }

    // Not found reject
    if (windowSeqIndex == -1)
    {
        safe_delete(frame);
        return;
    }

    if (frame->data == NULL)
    {
        safe_delete(frame);
        return;
    }

    // Frame founded and accepted

    // Update type on DataObject
    if (obj->type == 0)
        obj->type = frame->type;

    if (frame->type == Githyanki::END)
    {
        lout << "Finish frame received\n\tSeq - " << frame->seq << endl
             << endl;
        endSeq = frame->seq;
        windowPlace[windowSeqIndex]->received = true;
        receivedFrames++;
        finalize(frame);
    }
    else
    {
        lout << "Data frame received:\n\tType - " << (frame->type == Githyanki::TEXT ? "Text" : "Media") << "\n\tSeq - " << frame->seq << endl;

        DataBlock *data = new DataBlock(frame->data, frame->sizeData);
        // fout << data->data;
        //  cout << windowPlace[windowSeqIndex]->posi << endl;
        windowData[windowPlace[windowSeqIndex]->posi + 1] = data;
        windowDataSize++;
        windowPlace[windowSeqIndex]->received = true;
        receivedFrames++;
    }

    if (receivedFrames == Githyanki::SEND_WINDOW_MAX)
        acknowledge();

    safe_delete(frame);

    if (windowDataSize == 256)
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

    while (window.endSeq != window.lastAck)
    {
        frame = new Frame();
        frame = myCon->receiveFrame();

        if (randomChance(100 - chanceLostFrame))
            window.bufferFrame(frame);
    }

    lout << "Finished" << endl;
    flushBuffer(window.windowData, window.windowDataSize);
    if (window.obj->type == Githyanki::FILE)
    {
        rename("logs/buffer.bin", window.obj->name);
    }

    return window.obj;
}

void Githyanki::Window::init()
{
    int i;
    for (i = 0; i < Githyanki::SEND_WINDOW_MAX; i++)
    {
        window[i] = i;
    }
    sendingFrames = 0;
    lastSeq = i - 1;
    firstNotFramedIndex = 0;
    endSeq = -1;
}

void Githyanki::Window::acknowledge(Githyanki::Ack *ack)
{
    // New frames
    int buffer[Githyanki::SEND_WINDOW_MAX];
    int iBuffer = 0; //
    int j;           // Quantity of not acked frames
    int i;           // Frames passed

    for (i = 0; i < Githyanki::SEND_WINDOW_MAX; i++)
    {
        if (window[i] == ack->seq && ack->type == Githyanki::NACK)
            break;

        // Ack frame
        lastSeq = (lastSeq + 1) % Githyanki::WINDOW_MAX;
        buffer[iBuffer] = lastSeq;
        safe_delete(frames[i]);
        iBuffer++;
        sendingFrames--;

        if (window[i] == ack->seq && ack->type == Githyanki::ACK)
        {
            i++;
            break;
        }
    }
    for (j = 0; i < Githyanki::SEND_WINDOW_MAX; i++, j++)
    {
        window[j] = window[i];
        frames[j] = frames[i];
    }

    for (int i = 0; i < iBuffer; i++)
    {
        window[j + i] = buffer[i];
    }

    firstNotFramedIndex = j;
    safe_delete(ack);
}

// Window Stuff
void Githyanki::Window::prepareFrames(Githyanki::DataObject *obj)
{
    char *dataBuffer;
    int qtyPrepared = 0;
    int i = firstNotFramedIndex;
    int seq, bytesToSend = 0;

    for (; i < Githyanki::SEND_WINDOW_MAX; i++)
    {
        // memset(dataBuffer, 0, Githyanki::DATA_SIZE_MAX);
        seq = window[firstNotFramedIndex];
        bytesToSend = obj->size - obj->bytesFramed;

        if (bytesToSend == 0)
        {
            if (obj->type == Githyanki::TEXT)
                frames[i] = new Frame(Githyanki::END, seq);
            else
                frames[i] = new Frame(obj->name, obj->nameSize, Githyanki::END, seq);
            endSeq = seq;
            firstNotFramedIndex++;
            qtyPrepared++;
            sendingFrames += qtyPrepared;
            return; // Acabou
        }
        if (bytesToSend > Githyanki::DATA_SIZE_MAX)
            bytesToSend = Githyanki::DATA_SIZE_MAX;

        dataBuffer = obj->getBytes(bytesToSend);
        obj->bytesFramed += bytesToSend;

        frames[i] = new Frame(dataBuffer, bytesToSend, obj->type, seq);
        firstNotFramedIndex++;
        qtyPrepared++;
    }
    sendingFrames += qtyPrepared;
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
    obj->frameQty = ceil(sizeof(obj->data) / Githyanki::DATA_SIZE_MAX);

    window.prepareFrames(obj);

    while (window.sendingFrames != 0)
    {
        otherCon->sendNFrames(window.sendingFrames, window.frames);

        // Wait to recieve Ack or Nack
        ack = myCon->waitAcknowledge();

        if (ack->type != Githyanki::TIMEOUT)
        {
            window.acknowledge(ack);
            if (window.endSeq == -1)
                window.prepareFrames(obj);
        }
    }
    lout << "Finished";

    return 1;
}
