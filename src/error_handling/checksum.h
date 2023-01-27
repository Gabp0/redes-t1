#ifndef __CHKS__
#define __CHKS__

#include <cstdint>

using namespace std;

namespace checksum
{
    uint8_t check8(char *data, size_t size);
    uint16_t check16(char *data, size_t size);
    bool isValid(char *data, size_t size, uint8_t check);
    bool isValid(char *data, size_t size, uint16_t check);
} // namespace checksum

#endif // !__CHKS__
