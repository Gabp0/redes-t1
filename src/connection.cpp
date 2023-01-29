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

using namespace std;
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

// int Connection::receiveMessage(int timeoutMillis, char *buffer, int tamanho_buffer)
// {
//     long long comeco = timestamp();
//     struct timeval timeout = {.tv_sec = 0, .tv_usec = timeoutMillis * 1000};
//     setsockopt(this->socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout));
//     int bytes_lidos;
//     do
//     {
//         bytes_lidos = recv(this->socket, buffer, tamanho_buffer, 0);
//         if (Githyanki::isValid(buffer, bytes_lidos, NULL))
//         {
//             return bytes_lidos;
//         }
//     } while (timestamp() - comeco <= timeoutMillis);
//     return -1;
// }

Frame* Connection::receiveFrame(){
    Frame *frameReceived = new Frame();

    int timeoutMillis = 10000;
    char buffer[Githyanki::FRAME_SIZE_MAX];
    int tamanho_buffer = Githyanki::FRAME_SIZE_MAX;

    long long comeco = timestamp();
    struct timeval timeout = {.tv_sec = 0, .tv_usec = timeoutMillis * 1000};
    setsockopt(this->socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout));
    int bytes_lidos;
    do
    {
        bytes_lidos = recv(this->socket, buffer, tamanho_buffer, 0);
        if (bytes_lidos == 1 && buffer[0] == Githyanki::SOH)
        {
            bytes_lidos = recv(this->socket, buffer, tamanho_buffer, 0);
        }
        if(Githyanki::isValid(buffer, tamanho_buffer, frameReceived))
        return frameReceived;
    } while (timestamp() - comeco <= timeoutMillis);
    return new Frame(Githyanki::TIMEOUT, 0);
}

void Connection::sendFrame(Frame *msg)
{
    // cout << this->socket << endl;
    char buffer[FRAME_SIZE_MAX];
    char soh[1];
    soh[0] = Githyanki::SOH;

    size_t size = msg->toBytes(buffer);

    // cout << "Size: " << size << endl << endl;
    // cout << "Frame: " << endl;

    // cout<< "Header: ";
    // for(size_t i = 0; i < 4;i++){
    //     printf("%x",buffer[i]);
    // }
    // cout << " Data:\"";
    // for(size_t i = 3; i < size -2;i++){
    //     cout << buffer[i];
    // }
    // printf("\" Checksum: %c",buffer[size-1]);
    // cout << endl << endl;

    ssize_t sentSOH = send(this->socket, soh, 1, 0);
    ssize_t sent = send(this->socket, buffer, size, 0);
    if (sent > 0 && sentSOH > 0)
    {
        cout << "sent" << endl;
    }
}

Githyanki::Ack Connection::waitAcknowledge()
{
    char buffer[Githyanki::FRAME_SIZE_MAX];
    Frame *frame;

    while (true)
    {
        frame = receiveFrame();

        Githyanki::Frame f = {};
        f.fromBytes(buffer);

        // Timeout
        if (frame->type == Githyanki::TIMEOUT)
        {
            Ack ack = {.type = TIMEOUT, .seq = 0};
            return ack;
        }

        // Frame received not Acknowledge
        if (f.type == Githyanki::AWK || f.type == Githyanki::NACK)
        {
            Ack ack = {.type = f.type, .seq = f.seq};
            return ack;
        }
        delete frame;
    }
}

int Connection::acknowledge(int sequence)
{
    Frame awk = Frame(AWK, sequence);
    char bytes[FRAME_SIZE_MAX];

    size_t size = awk.toBytes(bytes);

    ssize_t sent = send(this->socket, bytes, size, 0);
    return sent;
}

long long Connection::timestamp()
{
    struct timeval tp;
    gettimeofday(&tp, NULL);
    return tp.tv_sec * 1000 + tp.tv_usec / 1000;
}