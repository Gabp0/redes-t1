#include "chat/chat.h"

int main(int argc, char const *argv[])
{
    Chat cs(argv[1], argv[2]);

    cs.loadChat();

    return 0;
}
