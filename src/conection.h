#ifndef __CNT__
#define __CNT__

#include <string>

using namespace std;

class Conection
{
private:
    int socket;

public:
    Conection(string device);
    ~Conection(void);
    int receiveMessage(int timeoutMillis, char *buffer, int tamanho_buffer);
    void sendMessage(void *msg, size_t size);
};

#endif