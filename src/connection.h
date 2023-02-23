#ifndef __CNT__
#define __CNT__

#include <string>
#include "githyanki.h"
#include <mutex>

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
    Githyanki::Frame* receiveFrame();
    Githyanki::Frame* receiveRTS();
    
    Githyanki::Ack* waitAcknowledge();
    Githyanki::Ack* waitRequest(int timeout = 3);
    
    int receiveMessage(int timeoutMillis, char *buffer, int tamanho_buffer);
    int acknowledge(Ack ack);
    void sendFrame(Githyanki::Frame *frame);
    void setTimeout(int timeout);
    // Send first n frames from *frames
    void sendNFrames(int n, Githyanki::Frame **frames);
};

#endif
