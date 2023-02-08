#include "common.h"
#include <ctime>

fstream common::lout;

namespace common
{
    void initLog(string file)
    {
        lout = fstream{file, lout.binary | lout.trunc | lout.in | lout.out};
        if (!lout.is_open())
            std::cout << "failed to open " << file << '\n';
        return;
    }

    void closeLog()
    {
        lout.close();
    }

    bool randomChance(int y)
    {
        int x;
        x = 1 + std::rand() / ((RAND_MAX + 1u) / 100); // Note: 1+rand()%6 is biased
        if (x <= y)
            return true;
        return false;
    }

    int randomBetween(int x, int y)
    {
        if (x < y)
        {
            x += std::rand() / ((RAND_MAX + 1u) / (y - x + 1));
            return x;
        }
        return 0;
    }

    void randomSeed()
    {
        std::srand(std::time(nullptr));
    }

    int distWindow(int f, int l, int s)
    {
        if (f < l)
            return s - l + f;
        return f - l;
    }

    bool ackIf(int f, int l, int s, int c)
    {
        if (((f < l) && (s - 1 - l + f >= c)) || (((f > l)) && (f - l - 1 >= c)))
        {
            return 1;
        }
        return 0;
    }

    void flushBuffer(string **buffer, int size)
    {
        common::lout << endl
                     << "Flushing Data" << endl
                     << endl;
        for (int i = 0; i < size; i++)
        {
            cout << buffer[i]->data();
            delete buffer[i];
        }
        cout << endl;
    }
}