#include "errors.h"
#include <iostream>
#include <vector>
#include <string>
#include "../utils/bits.h"
#include "../utils/trellis.h"

using namespace std;
using namespace errors;
using namespace bits;

char *errors::convolutionalCode(char *data, size_t size)
// gera o codigo convulacional 
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

char *errors::viterbiDecoder(char *data, size_t size)
// decodifica o codigo convulacional usando o algoritmo de viterbi
{
    vector<bit> data_bits(fromChar(data, size));
    Trellis trellis;

    bit group[2];
    for (size_t i = 0; i < data_bits.size(); i += 2)
    {
        // trabalha com grupos de dois bits por vez
        group[0] = data_bits.at(i + 1);
        group[1] = data_bits.at(i);

        trellis.makeTransition(group);
    }

    return toChar(trellis.getOptimalPath());
}
