#ifndef __CTSCR__
#define __CTSCR__

#include <ncurses.h>

class ChatScreen
{
private:
    WINDOW *chat_box;
    WINDOW *chat_history;
    int row, col;
    int history_cursor;

    void printToHistory(char *input, char *user);

public:
    ChatScreen();
    ~ChatScreen();
    void read(void);
};

#endif