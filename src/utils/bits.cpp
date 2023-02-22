#include "bits.h"
#include <vector>
#include <random>

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
    vector<bit> data_bits = fromChar(data, size);

    // random number generator
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distr(0, data_bits.size());

    for (size_t i = 0; i < (data_bits.size() / 25); i++)
    {
        int pos = distr(gen);
        data_bits.at(pos) = !data_bits.at(pos);
    }

    return toChar(data_bits);
}

char *bits::toChar(vector<bit> input)
// converte um vetor de bits para um vetor de char
{
    char *output = new char[input.size()];

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