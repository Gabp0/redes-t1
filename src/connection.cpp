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
#include "./error_handling/errors.h"


using namespace std;
using namespace common;
using namespace Githyanki;

int timeoutMillis = 5000;
int timeoutMin = 5000;

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

    int bytes_lidos;
    char buffer[Githyanki::FRAME_SIZE_MAX] = "";
    int tamanho_buffer = Githyanki::FRAME_SIZE_MAX;
    long long comeco = timestamp();
    struct timeval timeout = {.tv_sec = timeoutMillis / 1000, .tv_usec = (timeoutMillis % 1000) * 1000};
    setsockopt(this->socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout));
    // lout << endl
    //      << "Reading socket" << endl;
    do
    {
        bytes_lidos = recv(this->socket, buffer, tamanho_buffer, 0);
        if (buffer[0] == Githyanki::SOH)
        {
            bytes_lidos = recv(this->socket, buffer, tamanho_buffer, 0);
        }
        if (Githyanki::isValid(buffer, bytes_lidos, frameReceived))
        {
            timeoutMillis = timeoutMin;
            //frameReceived->toString();
            return frameReceived;
        }
    } while (timestamp() - comeco <= timeoutMillis);

    frameReceived->type = Githyanki::TIMEOUT;
    int t = randomBetween(2, 3);
    lout << "Timeout * " << t << endl;
    timeoutMillis *= t;
    lout << "Socket Timeout" << endl;
    return frameReceived;
}

void Connection::sendFrame(Frame *frame)
{
    char buffer[FRAME_SIZE_MAX] = "";
    char soh[16] = "63";

    size_t size = frame->toBytes(buffer);

    ssize_t sentSOH = send(this->socket, soh, 16, 0);
    ssize_t sent = send(this->socket, buffer, size, 0);

    if (sent > 0 && sentSOH > 0)
    {
        if (frame->type == Githyanki::END)
        {
            lout << "End frame sended:"
                 << "\n\tSeq - " << frame->seq << endl;
        }
        else if (frame->type == Githyanki::FILE || frame->type == Githyanki::TEXT)
        {
            lout << "Data frame sended:"
                 << "\n\tType - " << (frame->type == Githyanki::TEXT ? "Text" : "File") << "\n\tSeq - " << frame->seq << endl;
        }
        else
            lout << (frame->type == Githyanki::NACK ? "N-ACK" : "ACK") << " frame sended:"
                 << "\n\tSeq - " << frame->seq << endl;
    }
}


Githyanki::Ack *Connection::waitAcknowledge()
{
    Frame *frame = NULL;
    Ack *ack = new Ack(0, 0);

    lout << endl
         << "Waiting for ack" << endl;
    while (true)
    {
        frame = receiveFrame();

        if (frame != NULL)
        {
            // Timeout
            if (frame->type == Githyanki::TIMEOUT)
            {
                lout << "\tTimeout" << endl
                     << endl;
                ack->seq = 0;
                ack->type = TIMEOUT;
                return ack;
            }

            // Frame received not Acknowledge
            if (frame->type == Githyanki::ACK || frame->type == Githyanki::NACK)
            {
                ack->seq = frame->seq;
                ack->type = frame->type;
                lout << "\tReceived\n\t" << (ack->type == Githyanki::ACK ? "Ack: " : "Nack: ") << ack->seq << endl
                     << endl;
                return ack;
            }
            safe_delete(frame);
        }
    }
}

Githyanki::Ack *Connection::waitRequest()
{
    Frame *frame = NULL;
    Ack *ack = new Ack(0, 0);

    lout << endl
         << "Waiting for ack" << endl;
    while (true)
    {
        frame = receiveFrame();

        if (frame != NULL)
        {
            // Timeout
            if (frame->type == Githyanki::TIMEOUT)
            {
                lout << "\tTimeout" << endl
                     << endl;
                ack->seq = 0;
                ack->type = TIMEOUT;
                return ack;
            }

            // Frame received not Acknowledge
            if (frame->type == Githyanki::RTS || frame->type == Githyanki::CTS)
            {
                ack->seq = frame->seq;
                ack->type = frame->type;
                lout << "\tReceived\n\t" << (ack->type == Githyanki::ACK ? "Ack: " : "Nack: ") << ack->seq << endl
                     << endl;
                return ack;
            }
            safe_delete(frame);
        }
    }
}

int Connection::acknowledge(Ack ack)
{
    Frame *ackFrame;
    lout << endl
         << "Sending " << (ack.type == Githyanki::NACK ? "N-A" : "A") << "cknowledge: " << ack.seq << endl
         << endl;

    ackFrame = new Frame(ack.type, ack.seq);
    sendFrame(ackFrame);

    return 1;
}


void Connection::setTimeout(int timeout)
{
    timeoutMin = timeout;
    timeoutMillis = timeoutMin;
}

long long Connection::timestamp()
{
    struct timeval tp;
    gettimeofday(&tp, NULL);
    return tp.tv_sec * 1000 + tp.tv_usec / 1000;
}

void Connection::sendNFrames(int n, Githyanki::Frame **frames)
{
    for (int i = 0; i < n; i++)
        sendFrame(frames[i]);
}