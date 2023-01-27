#include "convolution.h"
#include <iostream>
#include <vector>
#include <cstring>

using namespace std;

vector<bool> fromChar(char *data, size_t size)
{
    vector<bool> output;

    for (size_t i = 0; i < size; i++) 
    {
        char c = data[i];
        for (int j = 0; j < 8; j++) // iterate over bits
        {
            bool bit = ((c >> (7 - j)) & 1);
            output.push_back(bit);
        }
    }

    return output;
}

char *toChar(vector<bool> bool_bits)
{
    char *output = new char[bool_bits.size()];
    
    char cbyte = 0x0;
    int counter = 0;
    int i = 0;
    for (bool bit : bool_bits)
    {
        cbyte = (cbyte << 1) | bit;
        counter += 1;
        if (counter == 8)
        {
            strcpy(&output[i], &cbyte);
            counter = 0;
            i++;
        }
    }

    return output;
}

char *convolution::convolution(char *data, size_t size)
{
    vector<bool> output;
    bool s0 = 0, s1 = 0;

    vector<bool> bool_bits(fromChar(data, size));

    for (bool in : bool_bits)
    {
        bool c0 = (in != s1);
        bool c1 = (c0 != s0);

        cout << "in: " << in << " s0: " << s0 << " s1: " << s1 << " c0: " << c0 << " c1: " << c1 << endl;

        s1 = s0;
        s0 = in;

        output.push_back(c0);
        output.push_back(c1);
    }
    
    return toChar(output);
}
