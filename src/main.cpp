#include "githyanki.h"
#include <string>
#include <iostream>

using namespace std;

int main(int argc, const char **argv)
{
    std::string message = "nao augneto mais esse trabalho";
    const char *messagec = message.c_str();
    Githyanki::frame *msg = Githyanki::createFrame(messagec, message.length(), Githyanki::DATA, 2);

    msg->toBytes();

    return 0;
}