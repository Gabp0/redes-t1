#ifndef __CNT__
#define __CNT__

#include <string>

using namespace std;

class Connection
{
private:
    int socket;

    long long timestamp(void);

public:
    Connection(string device);
    ~Connection(void);
    int receiveMessage(int timeoutMillis, char *buffer, int tamanho_buffer);
    void sendMessage(void *msg, size_t size);
};

#endif