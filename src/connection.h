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

    long long timestamp();

public:
    Connection(string device);
    ~Connection(void);
    int acknowledge(Ack ack);
    Githyanki::Ack* waitAcknowledge();
    int receiveMessage(int timeoutMillis, char *buffer, int tamanho_buffer);
    void sendFrame(Githyanki::Frame *frame);
    Githyanki::Frame* receiveFrame();
    void setTimeout(int timeout);
    Githyanki::Ack* waitRequest();
    // Send first n frames from *frames
    void sendNFrames(int n, Githyanki::Frame **frames);
};

#endif
