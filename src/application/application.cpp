#include "application.h"
#include <string>
#include "../connection.h"
#include "../githyanki.h"
#include "../utils/common.h"

using namespace std;

Application::Application(string server, string client)
{
    this->serverCon = new Connection(server);
    this->clientCon = new Connection(client);

    this->clientCon->setTimeout(1000);
    common::initLog("logs/clientLog.bin");
    common::initOutputFile("logs/buffer.bin");

    common::randomSeed();
}

Application::~Application()
{
    common::closeLog();
}

void Application::sendString(string *text)
{
    Githyanki::DataObject msg = {};
    msg.data = (char *)text->data();
    msg.type = Githyanki::TEXT;
    msg.myCon = this->serverCon;
    msg.otherCon = this->clientCon;
    msg.size = text->size();
    msg.bytesFramed = 0;

    this->serverCon->setTimeout(2000);
    common::initLog("logs/serverLog.bin");
    Githyanki::establishConnection(&msg);
}

void Application::sendFile(string filePath, string fileName)
{
    long sz = common::initInputFile(filePath);

    Githyanki::DataObject msg = {};
    msg.file = common::fin;

    // msg.data = (char *)text->data();
    msg.type = Githyanki::FILE;
    msg.myCon = this->serverCon;
    msg.otherCon = this->clientCon;
    msg.size = sz;
    msg.bytesFramed = 0;
    msg.name = (char *)fileName.data();
    msg.nameSize = fileName.size();

    this->serverCon->setTimeout(2000);
    common::initLog("logs/serverLog.bin");
    Githyanki::establishConnection(&msg);
}

int Application::listen(void)
{
    return Githyanki::listenToConnection(this->serverCon, this->clientCon);
}

void Application::recv(void)
{
    Githyanki::SlidingWindowReceive(this->clientCon, this->serverCon);
}