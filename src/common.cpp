#include "common.h"
#include <ctime>
#include "githyanki.h"

fstream common::lout;
fstream common::fout;
std::FILE *common::foutBinary;
std::FILE *common::fin;

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

    void initOutputFile(string file)
    {
        fout = fstream{file, fout.binary | fout.trunc | fout.in | fout.out};
        foutBinary = fopen(file.data(), "wb");
        // fin = fopen(filePath.data(), "wb");
        if (!fout.is_open())
            std::cout << "failed to open " << file << '\n';
        return;
    }

    void closeOutputFile()
    {
        fout.close();
    }

    long initInputFile(string filePath)
    {
        // fin = fstream{filePath, fout.binary | fout.trunc | fout.in | fout.out};
        fin = fopen(filePath.data(), "rb");
        if (fin == NULL)
        {
            std::cout << "failed to open " << filePath << '\n';
            return -1;
        }
        fseek(fin, 0L, SEEK_END);
        long sz = ftell(fin);
        fseek(fin, 0L, SEEK_SET);
        return sz;
    }

    void closeInputFile()
    {
        fclose(fin);
        // fin.close();
    }

    // void flushBuffer(Githyanki::DataBlock *buffer[], int size)
    // {

    //     common::lout << endl
    //                  << "Flushing Data" << endl
    //                  << endl;
    //     for (int i = 0; i < size; i++)
    //     {
    //         //
    //         if (buffer[i] != NULL && buffer[i]->data != NULL)
    //         {
    //             // common::fout << buffer[i]->data;
    //             fwrite(buffer[i]->data, 1, buffer[i]->size, foutBinary);
    //             safe_delete(buffer[i]);
    //         }
    //         else
    //         {
    //             cout << " NULL " << i << endl;
    //         }
    //     }
    // }

}