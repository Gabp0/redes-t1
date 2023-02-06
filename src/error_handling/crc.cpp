#include "errors.h"
#include "../utils/bits.h"
#include <iostream>

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

uint16_t errors::crc16(char *data, size_t size)
{
    uint16_t crc = 0xFFFF;

    for (size_t i = 0; i < size; ++i)
    {
        uint8_t pos = (crc >> 8) ^ data[i];
        crc = (crc << 8) ^ LOOKUP_TABLE_16[pos];
    }

    return crc;
}
