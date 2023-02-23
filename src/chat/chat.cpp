#include "chat.h"
#include <ncurses.h>
#include <locale.h>
#include <string>
#include <vector>
#include <bits/stdc++.h>
#include <ctime>
#include <thread>
#include "../application/application.h"

using namespace std;

WINDOW *create_newwin(int height, int width, int starty, int startx, chtype color_pair)
{
    WINDOW *new_win = newwin(height, width, starty, startx);
    wbkgd(new_win, color_pair);

    return (new_win);
}

Chat::Chat(string interface1, string interface2)
{
    this->app = new Application(interface1, interface2);

    setlocale(LC_ALL, "");
    initscr();
    keypad(stdscr, true);
    intrflush(stdscr, 0);
    start_color();

    init_pair(1, COLOR_BLACK, COLOR_RED);
    init_pair(2, COLOR_BLACK, COLOR_GREEN);

    getmaxyx(stdscr, this->row, this->col);
    this->history_cursor = 1;
    this->canReceive = true;
    this->receiving = false;
    this->finish = false;

    refresh();

    this->hist_border = create_newwin(this->row - 6, this->col, 0, 0, COLOR_PAIR(2));
    box(this->hist_border, 0, 0);

    this->chat_history = create_newwin(this->row - 8, this->col - 2, 1, 1, COLOR_PAIR(2));

    this->chat_border = create_newwin(5, this->col, this->row - 5, 0, COLOR_PAIR(1));
    box(this->chat_border, 0, 0);

    this->chat_box = create_newwin(3, this->col - 2, this->row - 4, 1, COLOR_PAIR(1));

    scrollok(this->chat_history, true);
    scrollok(this->hist_border, true);

    wrefresh(this->hist_border);
    wrefresh(this->chat_history);
    wrefresh(this->chat_border);
    wrefresh(this->chat_box);
}

Chat::~Chat()
{
    delwin(this->chat_history);
    delwin(this->hist_border);
    delwin(this->chat_box);
    delwin(this->chat_border);
    endwin();
}

void Chat::receiveThread(void)
{
    while (!this->finish)
    {
        if (this->canReceive)
        {
            this->receiving = this->app->listen();
        }

        if (this->receiving)
        {
            this->app->recv();
            this->receiving = false;
        }

        // this_thread::sleep_for(chrono::milliseconds(100));
    }
}

int Chat::loadChat(void)
{
    thread th1(receiveThread);

    while (!this->finish)
    {
        readFromUser();
    }

    th1.join();
}

void Chat::printToHistory(char *input, char *user)
{
    time_t end_time = chrono::system_clock::to_time_t(chrono::system_clock::now());
    wmove(this->hist_border, this->history_cursor, 0);

    time_t ct;
    time(&ct);
    struct tm *lt = localtime(&ct);

    wprintw(this->hist_border, " %d:%d:%d %s : %s\n", lt->tm_hour, lt->tm_min, lt->tm_sec, user, input);

    this->history_cursor++;
    box(this->hist_border, 0, 0);

    wrefresh(this->chat_history);
    wrefresh(this->hist_border);
}

bool Chat::readFromUser()
{
    char buffer[1024];

    wmove(this->chat_box, 0, 0);
    wgetstr(this->chat_box, buffer);

    while (receiving)
    { // halt the read until finish receiving
        this_thread::sleep_for(chrono::milliseconds(100));
    }

    delwin(this->chat_box);
    this->chat_box = create_newwin(3, this->col - 2, this->row - 4, 1, COLOR_PAIR(1));
    wrefresh(this->chat_box);

    string input(buffer);

    if (input[0] == '/')
    {

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
            this->canReceive = false;
            // arrumar o nome dps
            app->sendFile(substrs.at(1), "poggers.bin");
            this->canReceive = true;
        }
        else if (cmd.compare("/quit") == 0)
        {
            this->finish = true;
            return false;
        }
    }
    else
    {
        this->canReceive = false;
        this->app->sendString(&input);
        printToHistory(buffer, getenv("USER"));
        this->canReceive = true;
    }

    wrefresh(this->hist_border);
    wrefresh(this->chat_box);
    refresh();

    return true;
}

void Chat::receive(void)
{
    char str[] = "alguma coisa";
    char user[] = "gab2";

    while (1)
    {
        printToHistory(str, user);
        sleep(10);
    }
}
