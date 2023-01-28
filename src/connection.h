#ifndef __CNT__
#define __CNT__

#include <string>
#include "githyanki.h"

using namespace std;

class Connection
{
private:
    int socket;

    long long timestamp(void);

public:
    Connection(string device);
    ~Connection(void);
    int acknowledge(int sequence);
    int waitAcknowledge();
    int receiveMessage(int timeoutMillis, char *buffer, int tamanho_buffer);
    void sendMessage(Githyanki::Frame msg);
};

#endif