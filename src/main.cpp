#include "githyanki.h"
#include "connection.h"
#include "common.h"
#include <string>
#include <iostream>
#include <algorithm>

using namespace std;

bool cmdOptionExists(char **begin, char **end, const std::string &option)
{
        return std::find(begin, end, option) != end;
}

void sendString(string *text, Connection *serverCon, Connection *clientCon)
{
        Githyanki::DataObject msg = {};
        msg.data = (char *)text->data();
        msg.type = Githyanki::TEXT;
        msg.myCon = serverCon;
        msg.otherCon = clientCon;
        msg.size = text->size();
        msg.bytesFramed = 0;

        serverCon->setTimeout(2000);
        common::initLog("logs/serverLog.bin");
        Githyanki::SlidingWindowSend(&msg);
}

void sendFile(string filePath, string fileName, Connection *serverCon, Connection *clientCon)
{
        long sz = common::initInputFile(filePath);

        Githyanki::DataObject msg = {};
        msg.file = common::fin;

        // msg.data = (char *)text->data();
        msg.type = Githyanki::FILE;
        msg.myCon = serverCon;
        msg.otherCon = clientCon;
        msg.size = sz;
        msg.bytesFramed = 0;
        msg.name = fileName.data();
        msg.nameSize = fileName.size();

        serverCon->setTimeout(2000);
        common::initLog("logs/serverLog.bin");
        Githyanki::SlidingWindowSend(&msg);
}

void recv(Connection *serverCon, Connection *clientCon)
{
        clientCon->setTimeout(1000);
        common::initLog("logs/clientLog.bin");
        common::initOutputFile("logs/buffer.bin");
        Githyanki::SlidingWindowReceive(clientCon, serverCon);
}

int main(int argc, char **argv)
{
        Connection serverCon("lo1");
        Connection clientCon("lo2");

        common::randomSeed();

        if (cmdOptionExists(argv, argv + argc, "-s"))
        {
                string text = "Olha gostario de pedir um hambuegerer sem feijao, obrigado";
                // sendFile("arquivos/mosaico_empy.png", "mosaico_empy.png", &serverCon, &clientCon);
                // sendFile("logs/text", "text", &serverCon, &clientCon);
                sendString(&text, &serverCon, &clientCon);
        }
        else if (cmdOptionExists(argv, argv + argc, "-r"))
        {
                recv(&serverCon, &clientCon);
        }

        common::closeLog();

        return 0;
}
