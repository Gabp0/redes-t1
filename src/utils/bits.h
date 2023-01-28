#ifndef __BITS__
#define __BITS__

#include <vector>

using namespace std;

namespace bits
{
    typedef bool bit;

    vector<bit> fromChar(char *data, size_t size);
    char *toChar(vector<bit> input);
    char *flip(char *data, size_t size);
    int hammingDistance(bit x[2], bit y[2]);
} // namespace bits

#endif
