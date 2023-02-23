#include "application.h"
#include <string>
#include <sstream>
#include "../connection.h"
#include "../githyanki.h"
#include "../utils/common.h"

using namespace std;

Application::Application(string myCon, string otherCon)
{
    this->myCon = new Connection(myCon);
    this->otherCon = new Connection(otherCon);

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
    msg.myCon = this->myCon;
    msg.otherCon = this->otherCon;
    msg.size = text->size();
    msg.bytesFramed = 0;

    this->myCon->setTimeout(2000);
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
    msg.myCon = this->myCon;
    msg.otherCon = this->otherCon;
    msg.size = sz;
    msg.bytesFramed = 0;
    msg.name = (char *)fileName.data();
    msg.nameSize = fileName.size();

    this->myCon->setTimeout(2000);
    common::initLog("logs/serverLog.bin");

    Githyanki::establishConnection(&msg);
}

int Application::listen(void)
{
    return Githyanki::listenToConnection(this->myCon, this->otherCon);
}

string Application::recv(void)
{
    this->otherCon->setTimeout(1000);
    common::initLog("logs/clientLog.bin");
    common::initOutputFile("logs/buffer.bin");

    Githyanki::DataObject *obj = Githyanki::SlidingWindowReceive(this->otherCon, this->myCon);

    if (obj->type == Githyanki::FILE)
    {
        return "---Arquivo recebido, salvo em: " + string(obj->name) + "---";
    }

    if (obj->type == Githyanki::TEXT)
    {
        ifstream t("logs/buffer.bin");
        stringstream buffer;
        buffer << t.rdbuf();

        return buffer.str();
    }

    return "";
}