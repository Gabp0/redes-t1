#include "chat.h"
#include <thread>

void receive(Chat *cs)
{
    cs->receive();
}

int main(int argc, char const *argv[])
{
    Chat cs("lo1", "lo2");

    std::thread th1(receive, &cs);

    while (cs.readFromUser())
    {
    }

    th1.join();

    return 0;
}
