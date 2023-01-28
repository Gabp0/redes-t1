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
#include "checksum.h"

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

int Connection::receiveMessage(int timeoutMillis, char *buffer, int tamanho_buffer)
{
    long long comeco = timestamp();
    struct timeval timeout = {.tv_sec = 0, .tv_usec = timeoutMillis * 1000};
    setsockopt(this->socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout));
    int bytes_lidos;
    do
    {
        bytes_lidos = recv(this->socket, buffer, tamanho_buffer, 0);
        if (Githyanki::isValid(buffer, bytes_lidos))
        {
            return bytes_lidos;
        }
    } while (timestamp() - comeco <= timeoutMillis);
    return -1;
}

void Connection::sendMessage(frame msg)
{
    //cout << this->socket << endl;
    char buffer[FRAME_SIZE_MAX];
    char soh[1];
    soh[0] = Githyanki::SOH;

    size_t size = msg.toBytes(buffer);

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
    if (sent > 0)
    {
        cout << "sent" << endl;
    }
}

int Connection::waitAcknowledge(){
    char buffer[Githyanki::FRAME_SIZE_MAX];

    size_t size = c.receiveMessage(10000, buffer, Githyanki::FRAME_SIZE_MAX);

    Githyanki::Frame f = {};
    f.fromBytes(buffer);
    
    //Timeout
    if(size == -1){
        return -1;
    }

    //Frame received not Acknowledge
    if(f.type != Githyanki::AWK)
    return -2;

    
    return f.seq;
}

int Connection::acknowledge(int sequence){
    struct frame awk = frame(AWK, sequence);
    char bytes[FRAME_SIZE_MAX];

    size_t size = awk.toBytes(bytes);

    ssize_t sent = send(this->socket, bytes, size, 0);
    return sent;
}

long long Connection::timestamp() {
    struct timeval tp;
    gettimeofday(&tp, NULL);
    return tp.tv_sec*1000 + tp.tv_usec/1000;
}