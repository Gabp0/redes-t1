#include "githyanki.h"
#include "connection.h"
#include <string>
#include <iostream>
#include <algorithm>

using namespace std;

bool cmdOptionExists(char **begin, char **end, const std::string &option)
{
    return std::find(begin, end, option) != end;
}

int main(int argc, char **argv)
{
    Connection c("lo");

    if (cmdOptionExists(argv, argv + argc, "-s"))
    {
        cout << "sending" << endl;

        char txt[32] = "não aguento mais esse trabalho";
        Githyanki::frame *f = Githyanki::createFrame(txt, sizeof(txt), Githyanki::TEXT, 0);

        char bytes[Githyanki::FRAME_SIZE_MAX];
        size_t size = f->toBytes(bytes);
        c.sendMessage(bytes, size);
    }
    else if (cmdOptionExists(argv, argv + argc, "-r"))
    {
        cout << "receiving" << endl;
        char buffer[Githyanki::FRAME_SIZE_MAX];

        size_t size = c.receiveMessage(10000, buffer, Githyanki::FRAME_SIZE_MAX);
        cout << size << endl;

        Githyanki::frame *f = new Githyanki::frame;
        f->fromBytes(buffer);
    }

    return 0;
}