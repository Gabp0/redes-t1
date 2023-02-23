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

        if (cmdOptionExists(argv, argv + argc, "-s"))
        {
                Application app("lo1", "lo2");

                string text = "Olha gostario de pedir um hambuegerer sem feijao, obrigado";
                // app.send("arquivos/mosaico_empy.png", "mosaico_empy.png");
                // app.send("arquivos/ah.mp4", "eh.mp4");
                // sendFile("logs/text", "text", &serverCon, &clientCon);
                app.send(&text);
        }
        else if (cmdOptionExists(argv, argv + argc, "-r"))
        {
                Application app("lo2", "lo1");
                app.recv();
        }

        return 0;
}
