#include "chat_screen.h"
#include <ncurses.h>
#include <locale.h>
#include <string>
#include <vector>
#include <bits/stdc++.h>
#include <chrono>
#include <ctime>

using namespace std;

WINDOW *create_newwin(int height, int width, int starty, int startx)
{
    WINDOW *new_win = newwin(height, width, starty, startx);
    box(new_win, 0, 0);
    wrefresh(new_win);

    return (new_win);
}

ChatScreen::ChatScreen()
{
    setlocale(LC_ALL, "");
    initscr();
    keypad(stdscr, true);
    intrflush(stdscr, 0);
    start_color();

    init_pair(1, COLOR_BLACK, COLOR_RED);
    init_pair(2, COLOR_BLACK, COLOR_GREEN);

    getmaxyx(stdscr, this->row, this->col);
    this->history_cursor = 1;

    refresh();

    this->chat_history = create_newwin(this->row - 6, this->col, 0, 0);
    wbkgd(this->chat_history, COLOR_PAIR(2));
    wrefresh(this->chat_history);

    this->chat_box = create_newwin(4, this->col, this->row - 4, 0);
    wbkgd(this->chat_box, COLOR_PAIR(1));
    wrefresh(this->chat_box);

    wmove(this->chat_box, 0, 0);

    wgetch(this->chat_box);
}

ChatScreen::~ChatScreen()
{
    delwin(this->chat_history);
    delwin(this->chat_box);
    endwin();
}

void ChatScreen::printToHistory(char *input, char *user)
{
    time_t end_time = chrono::system_clock::to_time_t(chrono::system_clock::now());
    wmove(this->chat_history, this->history_cursor, 1);
    wprintw(this->chat_history, "%s : %s", user, input);

    this->history_cursor++;
    wmove(this->chat_box, 0, 0);
    wrefresh(this->chat_history);
    wrefresh(this->chat_box);
}

void ChatScreen::read()
{
    char buffer[1024];

    wgetstr(this->chat_box, buffer);

    string input(buffer);

    if (input[0] == '/')
    {
        wrefresh(this->chat_history);
        wrefresh(this->chat_box);
        stringstream inp_stream(input);
        vector<string> substrs;
        string s;
        while (getline(inp_stream, s, ' '))
        {
            substrs.push_back(s);
        }
        string cmd = substrs.at(0);

        if (cmd.compare("/send") == 0)
        {
            // cool logic
        }

        if (cmd.compare("/quit") == 0)
        {
            exit(EXIT_SUCCESS);
        }
    }
    else
    {
        printToHistory(buffer, getenv("USER"));
    }
}
