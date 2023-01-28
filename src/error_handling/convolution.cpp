#include "convolution.h"
#include <iostream>
#include <vector>
#include <string>
#include "../utils/bits.h"
#include "../utils/trellis.h"

using namespace std;
using namespace convolution;
using namespace bits;

char *convolution::code(char *data, size_t size)
{
    vector<bit> output;
    bit s0 = 0, s1 = 0;

    vector<bit> data_bits(fromChar(data, size));

    for (bit in : data_bits)
    {
        bit c0 = (in != s1);
        bit c1 = (c0 != s0);

        s1 = s0;
        s0 = in;

        output.push_back(c1);
        output.push_back(c0);
    }

    return toChar(output);
}

char *convolution::viterbiDecoder(char *data, size_t size)
{
    vector<bit> data_bits(fromChar(data, size));
    Trellis trellis;

    bit group[2];
    for (size_t i = 0; i < data_bits.size(); i += 2)
    {
        // works with groups of 2 bits
        group[0] = data_bits.at(i + 1);
        group[1] = data_bits.at(i);

        trellis.makeTransition(group);
    }

    return toChar(trellis.getOptimalPath());
}