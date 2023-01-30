#ifndef __CNV__
#define __CNV__

#include <cstdint>
#include <vector>

using namespace std;

namespace convolution
// implementa o codigo convolucional e o algoritmo de viterbi para decodificar
{
    char *code(char *data, size_t size);
    char *viterbiDecoder(char *data, size_t size);
} // namespace convolution

#endif // !__CNV__
