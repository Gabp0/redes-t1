#ifndef __CTSCR__
#define __CTSCR__

#include <ncurses.h>
#include <string>
#include "../application/application.h"

using namespace std;

class Chat
{
private:
    WINDOW *chat_border;
    WINDOW *chat_box;
    WINDOW *hist_border;
    WINDOW *chat_history;
    int row, col;
    int history_cursor;
    Application *app;
    bool canReceive;
    bool receiving;

    bool finish;

    static void receiveThread(Chat *cs);
    void printToHistory(string input, string user);

public:
    Chat(string interface1, string interface2);
    ~Chat();
    int loadChat();
    bool readFromUser(void);
    void receive(void);
};

#endif