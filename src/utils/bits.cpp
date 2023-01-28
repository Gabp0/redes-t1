#include "bits.h"
#include <vector>

using namespace std;
using namespace bits;

vector<bit> bits::fromChar(char *data, size_t size)
{
    vector<bit> output;

    for (size_t i = 0; i < size; i++)
    {
        char c = data[i];
        for (int j = 0; j < 8; j++) // iterate over bits
        {
            bit bit = ((c >> (7 - j)) & 1);
            output.push_back(bit);
        }
    }

    return output;
}

char *bits::toChar(vector<bit> input)
{
    char *output = new char[input.size()];
    // cout << data_bits.size() << endl;

    char cbyte = 0x0;
    int counter = 0;
    int i = 0;
    for (bit b : input)
    {
        cbyte = (cbyte << 1) | b;
        counter += 1;
        if (counter == 8)
        {
            output[i] = cbyte;
            // cout << cbyte << endl;
            counter = 0;
            cbyte = 0x0;
            i++;
        }
    }

    return output;
}

int bits::hammingDistance(bit x[2], bit y[2])
{
    int distance = (x[0] == y[0]) ? 1 : 0;
    distance += (x[1] == y[1]) ? 1 : 0;

    return distance;
}