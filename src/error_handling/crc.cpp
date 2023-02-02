#include "errors.h"
#include "../utils/bits.h"

using namespace std;

uint8_t errors::crc8(char *data, size_t size)
{
    uint8_t crc = 0xFF;
    for (size_t i = 0; i < size; i++)
    {
        uint8_t cbyte = data[i];
        crc = LOOKUP_TABLE[cbyte ^ crc];
    }
    return crc;
}
