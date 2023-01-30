#include "bits.h"
#include <vector>
#include <iostream>

using namespace std;
using namespace bits;

vector<bit> bits::fromChar(char *data, size_t size)
// converte um vetor de char para um vetor de bits
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

char *bits::flip(char *data, size_t size)
// flipa bits aleatorios para testar
{
    vector<bit> input = fromChar(data, size);

    input.at(size / 4) = !input.at(size / 4);
    input.at(size * 3 / 4) = !input.at(size * 3 / 4);
    input.at(size / 20) = !input.at(size / 20);
    input.at(size * 9 / 10) = !input.at(size * 9 / 10);

    return toChar(input);
}

char *bits::toChar(vector<bit> input)
// converte um vetor de bits para um vetor de char
{
    size_t size = (input.size() / 8) + 1;
    char *output = new char[size];

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
// calcula a distancia de hamming entre um grupo de dois bits
{
    int distance = (x[0] == y[0]) ? 0 : 1;
    distance += (x[1] == y[1]) ? 0 : 1;

    return distance;
}