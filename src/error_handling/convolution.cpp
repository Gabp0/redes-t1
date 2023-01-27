#include "convolution.h"
#include <iostream>

using namespace std;

char *convolution::convolution(char *data, size_t size)
{
    char *output = new char[size * 2];
    bool s0 = 0, s1 = 0;

    for (size_t i = 0; i < size; i++)
    {
        char c = data[i];
        char o[2];
        cout << "current c: " << c << endl;
        for (int j = 0; j < 8; j++) // iterate over bits
        {
            bool in = ((c >> (7 - j)) & 1);

            bool c0 = (in != s1);
            bool c1 = (c0 != s0);

            cout << "in: " << in << " s0: " << s0 << " s1: " << s1 << " c0: " << c0 << " c1: " << c1 << endl;

            s1 = s0;
            s0 = in;

            o[j / 4] = (o[j / 4] << 2) | (c0 << 1) | c1;

            printf("o: %02x\n", o);
        }
        output[i * 2] = o[0];
        output[i * 2 + 1] = o[1];
    }

    return output;
}