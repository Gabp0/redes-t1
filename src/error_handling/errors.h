#ifndef __ERROS__
#define __ERROS__

#include <cstdint>

using namespace std;

namespace errors
// implementa o checksum para deteccao de erros
{
    const uint16_t POLINOMIAL = 0x14D// 101001101

    uint8_t checksum8(char *data, size_t size);
    uint16_t checksum16(char *data, size_t size);
    bool check8(char *data, size_t size, uint8_t check);
    bool check16(char *data, size_t size, uint16_t check);

    char *convolutionalCode(char *data, size_t size);
    char *viterbiDecoder(char *data, size_t size);

    uint8_t crc8(char *data, size_t size);
    bool crc8check(char *data, size_t size, uint8_t check);
} // namespace erros

#endif
