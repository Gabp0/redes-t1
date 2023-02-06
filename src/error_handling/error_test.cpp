#include "errors.h"
#include "../utils/bits.h"
#include <string>
#include <cstring>
#include <iostream>

using namespace std;

int main(int argc, char const *argv[])
{
    string buffer;
    getline(cin, buffer);

    size_t size = buffer.size() + 1;
    char *data = new char[size];
    char *data_check = new char[size + 2];
    strncpy(data, buffer.c_str(), size);
    strncpy(data_check, buffer.c_str(), size);

    cout << "String original: " << data << endl;

    uint8_t check8 = errors::checksum8(data, size);
    printf("Checksum 8 bits = 0x%02x\n", check8);
    data_check[size] = check8;
    printf("Checagem do checksum 8 bits = 0x%02x\n", errors::checksum8(data_check, size + 1));

    uint16_t check16 = errors::checksum16(data, size);
    printf("Checksum 16 bits = 0x%04x\n", check16);
    data_check[size] = (check16 >> 8);
    data_check[size + 1] = (check16 & 0xFF);
    printf("Checagem do checksum 16 bits = 0x%04x\n", errors::checksum16(data_check, size + 2));

    uint8_t crc8 = errors::crc8(data, size);
    printf("CRC 8 bits = 0x%02x\n", crc8);
    data_check[size] = crc8;
    printf("Checagem do CRC 8 bits = 0x%02x\n", errors::crc8(data_check, size + 1));

    uint16_t crc16 = errors::crc16(data, size);
    printf("CRC 16 bits = 0x%04x\n", crc16);
    data_check[size] = (crc16 >> 8);
    data_check[size + 1] = (crc16 & 0xFF);
    printf("Checagem do CRC 16 bits = 0x%04x\n", errors::crc16(data_check, size + 2));

    char *conv = errors::convolutionalCode(data, size);
    cout << "Codigo convolucional = " << conv << endl;
    char *flipped = bits::flip(conv, size * 2);
    cout << "Codigo convolucional com bits flippados = " << flipped << endl;
    char *conv_dec = errors::viterbiDecoder(flipped, size * 2);
    cout << "Codigo convolucional decodificado = " << conv_dec << endl;

    delete[] conv;
    delete[] conv_dec;
    delete[] flipped;
    delete[] data;
    delete[] data_check;

    return 0;
}
