#include "errors.h"
#include "../utils/bits.h"

using namespace std;

uint8_t errors::crc8(char *data, size_t size)
{
    uint8_t crc = 0xFF;
    for (size_t i = 0; i < size; i++)
    {
        uint8_t cbyte = data[i];
        crc = LOOKUP_TABLE_8[cbyte ^ crc];
    }
    return crc;
}

uint8_t errors::crc16(char *data, size_t size)
{
    uint16_t crc = 0xFFFF;

    size_t s = 1;
    uint8_t nbyte;
    uint16_t cbyte = data[0];
    // adiciona um padding no incio se o tamanho for impar
    if ((size % 2) == 0)
    {
        cbyte <<= 8;
        nbyte = data[1];
        cbyte |= nbyte;
        s = 2;
    }
    crc = LOOKUP_TABLE_16[cbyte ^ crc];

    for (size_t i = s; i < size; i += 2)
    {
        cbyte = data[i];
        nbyte = data[i + 1];
        cbyte <<= 8;
        cbyte |= nbyte;

        crc = LOOKUP_TABLE_16[cbyte ^ crc];
    }
    return crc;
}
