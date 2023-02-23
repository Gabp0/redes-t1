#include "application/application.h"
#include "utils/common.h"
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
        Application app("lo1", "lo2");

        if (cmdOptionExists(argv, argv + argc, "-s"))
        {
                string text = "Olha gostario de pedir um hambuegerer sem feijao, obrigado";
                // sendFile("arquivos/mosaico_empy.png", "mosaico_empy.png", &serverCon, &clientCon);
                // sendFile("logs/text", "text", &serverCon, &clientCon);
                app.sendString(&text);
        }
        else if (cmdOptionExists(argv, argv + argc, "-r"))
        {
                app.recv();
        }

        return 0;
}
