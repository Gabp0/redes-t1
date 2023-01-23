#include "githyanki.h"
#include <string>
#include <iostream>

using namespace std;

int main(int argc, const char **argv)
{
    std::string message = "nao augneto mais esse trabalho";
    const char *messagec = message.c_str();
    Githyanki::frame *msg = Githyanki::createFrame(messagec, message.length(), Githyanki::DATA, 2);

    void *bytes = msg->toBytes();

    Githyanki::frame *fr = new Githyanki::frame;
    fr->fromBytes(bytes);

    return 0;
}