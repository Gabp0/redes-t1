#ifndef __CNV__
#define __CNV__

#include <cstdint>
#include <vector>

using namespace std;

namespace convolution
{
    char *code(char *data, size_t size);
    char *viterbiDecoder(char *data, size_t size);
} // namespace convolution

#endif // !__CNV__
