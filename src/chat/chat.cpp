#include "chat.h"
#include <ncurses.h>
#include <locale.h>
#include <string>
#include <vector>
#include <bits/stdc++.h>
#include <ctime>
#include <thread>
#include <sys/stat.h>
#include "../application/application.h"
#include "../githyanki.h"

using namespace std;

WINDOW *create_newwin(int height, int width, int starty, int startx, chtype color_pair)
{
    WINDOW *new_win = newwin(height, width, starty, startx);
    wbkgd(new_win, color_pair);

    return (new_win);
}

Chat::Chat(string myCon, string otherCon)
{
    this->app = new Application(myCon, otherCon);

    setlocale(LC_ALL, "");
    initscr();
    keypad(stdscr, true);
    intrflush(stdscr, 0);
    start_color();

    init_pair(1, COLOR_BLACK, COLOR_RED);
    init_pair(2, COLOR_BLACK, COLOR_GREEN);

    getmaxyx(stdscr, this->row, this->col);
    this->history_cursor = 0;
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

void Chat::receiveThread(Chat *cs)
{
    while (!cs->finish)
    {
        //&& !cs->app->sending
        if (cs->canReceive && !cs->receiving)
        {
            cs->receiving = cs->app->listen(&(cs->finish));
        }

        if (cs->receiving)
        {
            string rcv = cs->app->recv();
            cs->receiving = false;
            cs->printToHistory("Recebido: " + rcv);
        }

        this_thread::sleep_for(chrono::milliseconds(100));
    }
}

int Chat::loadChat(void)
{
    thread th1(receiveThread, this);

    while (!this->finish)
    {
        readFromUser();
    }

    th1.join();

    return 0;
}

void Chat::printToStatus(string input)
{
    wmove(stdscr, this->row - 6, 0);
    printw("                                                          "); // clear previuous message
    wmove(stdscr, this->row - 6, 0);
    printw("%s\n", input.c_str());

    refresh();
}

void Chat::printToHistory(string input)
{
    wmove(this->chat_history, this->history_cursor, 0);

    time_t ct;
    time(&ct);
    struct tm *lt = localtime(&ct);

    wprintw(this->chat_history, "%d:%d:%d %s\n", lt->tm_hour, lt->tm_min, lt->tm_sec, input.c_str());

    this->history_cursor++;

    wrefresh(this->chat_history);
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

    // redraw win to erase previuous message
    delwin(this->chat_box);
    this->chat_box = create_newwin(3, this->col - 2, this->row - 4, 1, COLOR_PAIR(1));
    wrefresh(this->chat_box);

    string input(buffer);

    if (input.size() > 0)
    {
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

            if (cmd.compare("/send") == 0) // send file
            {
                if (substrs.size() != 3)
                {
                    printToStatus("Número inválido de argumentos. Uso: /send {path} {filename}");
                }
                else
                {
                    string filename = substrs.at(1);
                    this->canReceive = false;
                    printToStatus("Enviando arquivo...");
                    int status = app->send(substrs.at(1), substrs.at(2));
                    printToStatus(app->getStatus() + ":" + to_string(status));
                    if (status == Githyanki::SUCESS)
                    {
                        printToHistory("'''Enviado arquivo " + filename + "'''");
                    }
                    this->canReceive = true;
                }
            }
            else if (cmd.compare("/quit") == 0 || cmd.compare("/q") == 0) // quit the program
            {
                this->finish = true;
                return false;
            }
            else
            {
                printToStatus("Comando " + cmd + " não encontrado: " + to_string(Githyanki::INV_CMD));
            }
        }
        else
        { // send message
            this->canReceive = false;
            printToStatus("Enviando mensagem...");
            int status = this->app->send(&input);
            printToStatus(app->getStatus() + ":" + to_string(status));
            if (status == Githyanki::SUCESS)
            {
                printToHistory("Enviado: " + input);
            }
            this->canReceive = true;
        }
    }

    wrefresh(this->hist_border);
    wrefresh(this->chat_box);
    refresh();

    return true;
}