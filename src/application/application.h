#ifndef __APPL__
#define __APPL__

#include <string>
#include "../connection.h"
#include <mutex>

using namespace std;

class Application
{
private:
    Connection *myCon;
    Connection *otherCon;
    string status_msg;

public:
    Application(string myCon, string otherCon);
    ~Application();
    string getStatus(void);

    int send(string *text);
    int send(string filePath, string fileName);
    
    int listen(bool *finish);
    int establishConnection();

    mutex sendingMutex;
    // mutex recvMutex;

    string recv(void);

    // bool sending;
};

#endif