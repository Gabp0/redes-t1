#include "connection.h"
#include <string>
#include <cstring>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdexcept>
#include <iostream>
#include "sockets/socket.h"
#include "githyanki.h"
#include "common.h"

using namespace std;
using namespace common;
using namespace Githyanki;

Connection::Connection(string device)
{
    char *device_c = new char[device.length() + 1];
    strcpy(device_c, device.c_str());
    this->socket = cria_raw_socket(device_c);
}

Connection::~Connection(void)
{
    close(this->socket);
}

Frame *Connection::receiveFrame()
{
    Frame *frameReceived = new Frame();

    int timeoutMillis = 5000;
    int bytes_lidos;
    char buffer[Githyanki::FRAME_SIZE_MAX];
    int tamanho_buffer = Githyanki::FRAME_SIZE_MAX;
    long long comeco = timestamp();
    struct timeval timeout = {.tv_sec = 0, .tv_usec = timeoutMillis * 1000};

    setsockopt(this->socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout));
    
    do
    {
        bytes_lidos = recv(this->socket, buffer, tamanho_buffer, 0);
        if (buffer[0] == Githyanki::SOH)
        {
            bytes_lidos = recv(this->socket, buffer, tamanho_buffer, 0);
        }
        if (Githyanki::isValid(buffer, bytes_lidos, frameReceived))
            return frameReceived;
    } while (timestamp() - comeco <= timeoutMillis);

    frameReceived = new Frame(Githyanki::TIMEOUT, 0);
    log << "Timeout" << endl;
    return frameReceived;
}

void Connection::sendFrame(Frame *frame)
{
    char buffer[FRAME_SIZE_MAX];
    char soh[16] = "63";

    size_t size = frame->toBytes(buffer);
    ssize_t sentSOH = send(this->socket, soh, 16, 0);
    ssize_t sent = send(this->socket, buffer, size, 0);
    
    if (sent > 0 && sentSOH > 0)
        log << "Sent: seq - " << frame->seq  << endl;
}

Githyanki::Ack* Connection::waitAcknowledge()
{
    Frame *frame = NULL;
    Ack *ack = new Ack(0,0);

    cout << "waiting for ack" << endl;
    while (true)
    {
        // cout << "hey" << endl;
        frame = receiveFrame();
        // cout << "Wait Ack" << endl
        //      << frame->toString() << endl;

        // Timeout
        if (frame->type == Githyanki::TIMEOUT)
        {
            cout << "Timeout" << endl;
            ack->seq = 0;
            ack->type = TIMEOUT;
            return ack;
        }

        // Frame received not Acknowledge
        if (frame->type == Githyanki::ACK || frame->type == Githyanki::NACK)
        {
            ack->seq = frame->seq;
            ack->type = frame->type;
            cout << "Ack: " << ack->type << " Seq: " << ack->seq << endl;
            return ack;
        }
        // if(frame != NULL)
        // delete frame;
    }
}

int Connection::acknowledge(int sequence, int nawc)
{
    Frame *ack;
    cout << "Ack" << endl;
    if (nawc)
        ack = new Frame(NACK, sequence);
    else
        ack = new Frame(ACK, sequence);

    sendFrame(ack);
    return 1;
}

long long Connection::timestamp()
{
    struct timeval tp;
    gettimeofday(&tp, NULL);
    return tp.tv_sec * 1000 + tp.tv_usec / 1000;
}