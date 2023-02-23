#ifndef __APPL__
#define __APPL__

#include <string>
#include "../connection.h"

using namespace std;

class Application
{
private:
    Connection *myCon;
    Connection *otherCon;

public:
    Application(string server, string client);
    ~Application();
    void sendString(string *text);
    void sendFile(string filePath, string fileName);
    int listen(void);
    string recv(void);
};

#endif