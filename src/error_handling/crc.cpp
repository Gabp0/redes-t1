#include "errors.h"
#include "../utils/bits.h"
#include <iostream>

using namespace std;

void errors::genCRCtable(uint8_t polynomial)
// gera a lookup table para um crc de 8 bits
{
    for (size_t i = 0; i < 256; i++)
    {
        uint8_t cbyte = i;
        for (size_t j = 0; j < 8; j++)
        {
            if (cbyte >> 7) // se bit mais significativo == 1
            {
                cbyte = (cbyte << 1) ^ polynomial;
            }
            else
            {
                cbyte <<= 1;
            }
        }
        printf("0x%02x, ", cbyte);
    }
    cout << endl;
}

void errors::genCRCtable(uint16_t polynomial)
// gera a lookup table para um crc de 16 bits
{
    for (size_t i = 0; i < 256; i++)
    {
        uint16_t cbyte = (i << 8);

        for (size_t j = 0; j < 8; j++)
        {
            if (cbyte >> 15) // se bit mais significativo == 1
            {
                cbyte = (cbyte << 1) ^ polynomial;
            }
            else
            {
                cbyte <<= 1;
            }
        }
        printf("0x%04x, ", cbyte);
    }
    cout << endl;
}

uint8_t errors::crc8slow(char *data, size_t size)
// calcula o crc 8 bits
{
    uint8_t crc = 0;

    for (size_t i = 0; i < size; i++)
    {
        crc ^= data[i];

        for (size_t j = 0; j < 8; j++)
        {
            if (crc >> 7) // se bit mais significativo == 1
            {
                crc = ((crc << 1) ^ POLYNOMIAL_8);
            }
            else
            {
                crc <<= 1;
            }
        }
    }

    return crc;
}

uint16_t errors::crc16slow(char *data, size_t size)
// calcula o crc 16 bits
{
    uint16_t crc = 0;

    for (size_t i = 0; i < size; i++)
    {
        crc ^= (data[i] << 8); //

        for (size_t j = 0; j < 8; j++)
        {
            if (crc >> 15) // se bit mais significativo == 1
            {
                crc = ((crc << 1) ^ POLYNOMIAL_16);
            }
            else
            {
                crc <<= 1;
            }
        }
    }

    return crc;
}

uint8_t errors::crc8(char *data, size_t size)
// calcula o crc 8 bits usando uma lookup table
{
    uint8_t crc = 0;
    for (size_t i = 0; i < size; i++)
    {
        uint8_t cbyte = data[i];
        crc = LOOKUP_TABLE_8[cbyte ^ crc];
    }
    return crc;
}

uint16_t errors::crc16(char *data, size_t size)
// calcula o crc 16 bits usando uma lookup table
{
    uint16_t crc = 0;

    for (size_t i = 0; i < size; ++i)
    {
        uint8_t pos = (crc >> 8) ^ data[i];
        crc = (crc << 8) ^ LOOKUP_TABLE_16[pos];
    }

    return crc;
}
