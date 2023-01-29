#ifndef __CNT__
#define __CNT__

#include <string>
#include "githyanki.h"

using namespace std;
using namespace Githyanki;

class Connection
{
private:
    int socket;

    long long timestamp(void);

public:
    Connection(string device);
    ~Connection(void);
    int acknowledge(int sequence);
    Githyanki::Ack waitAcknowledge();
    int receiveMessage(int timeoutMillis, char *buffer, int tamanho_buffer);
    void sendFrame(Githyanki::Frame *msg);
    Githyanki::Frame receiveFrame();
};

#endif