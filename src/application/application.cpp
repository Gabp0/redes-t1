#include "application.h"
#include <string>
#include <sstream>
#include "../connection.h"
#include "../githyanki.h"
#include "../utils/common.h"
#include <thread>
#include <sys/stat.h>

using namespace std;

Application::Application(string myCon, string otherCon)
{
    this->myCon = new Connection(myCon);
    this->otherCon = new Connection(otherCon);

    common::initLog("logs/" + myCon + "Log.bin");

    common::randomSeed();
}

Application::~Application()
{
    common::closeLog();
}

int Application::send(string *text)
{
    Githyanki::DataObject msg = {};
    msg.data = (char *)text->data();
    msg.type = Githyanki::TEXT;
    msg.myCon = this->myCon;
    msg.otherCon = this->otherCon;
    msg.size = text->size();
    msg.bytesFramed = 0;

    this->myCon->setTimeout(2000);

    sendingMutex.lock();
    int status = establishConnection();
    if (status == Githyanki::SUCESS)
    {
        status = Githyanki::SlidingWindowSend(&msg);
    }
    sendingMutex.unlock();

    return status;
}

int Application::send(string filePath, string fileName)
{
    struct stat buffer;
    if (stat(fileName.c_str(), &buffer) != 0)
    {
        return Githyanki::FILE_NF;
    }

    long sz = common::initInputFile(filePath);

    if (sz < 0)
    {
        return Githyanki::FILE_OE;
    }

    common::lout << sz << endl;
    Githyanki::DataObject msg = {};
    msg.file = common::fin;

    msg.type = Githyanki::FILE;
    msg.myCon = this->myCon;
    msg.otherCon = this->otherCon;
    msg.size = sz;
    msg.bytesFramed = 0;
    msg.name = (char *)fileName.data();
    msg.nameSize = fileName.size();

    this->myCon->setTimeout(2000);

    int status = 200;
    sendingMutex.lock();
    if (establishConnection())
    {
        status = Githyanki::SlidingWindowSend(&msg);
    }
    sendingMutex.unlock();

    return status;
}

int Application::establishConnection()
{
    Ack *request;

    Frame rtsFrame = Frame(Githyanki::RTS, 0);
    otherCon->sendFrame(&rtsFrame);
    request = myCon->waitRequest(25); // Loop ate timeout informado ou CTS received

    if (request->type == Githyanki::CTS)
    {
        safe_delete(request);
        common::lout << "Starting transmission:" << endl;
        return Githyanki::SUCESS;
    }
    else if (request->type == Githyanki::TIMEOUT)
    {
        safe_delete(request);
        common::lout << "CTS Timeout" << endl;
        return Githyanki::NO_CONECTION;
    }

    return 0;
}

int Application::listen(bool *finish)
{
    while (true)
    {
        sendingMutex.lock();
        Ack *request = myCon->waitRequest();
        sendingMutex.unlock();

        if (request->type == Githyanki::RTS)
        {
            common::lout << "Request frame received:"
                         << "\n\tType - RTS" << endl;
            Frame ctsFrame = Frame(Githyanki::CTS, 0);
            otherCon->sendFrame(&ctsFrame);
            safe_delete(request);
            return 1;
        }
        if (*finish)
        {
            return 0;
        }

        this_thread::sleep_for(chrono::milliseconds(100));
    }
    return 0;
}

string Application::recv(void)
{
    this->otherCon->setTimeout(1000);

    common::initOutputFile("logs/buffer.bin");

    Githyanki::DataObject *obj = Githyanki::SlidingWindowReceive(this->myCon, this->otherCon);

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