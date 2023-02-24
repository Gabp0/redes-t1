#include "chat/chat.h"
#include <iostream>

int main(int argc, char const *argv[])
{
    if (!argv[1])
    {
        std::cout << "Uso: /chat {interface}" << endl;
        exit(EXIT_FAILURE);
    }

    Chat cs(argv[1], argv[2]);

    cs.loadChat();

    return EXIT_SUCCESS;
}
