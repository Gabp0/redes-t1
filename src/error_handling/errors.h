#ifndef __ERROS__
#define __ERROS__

#include <cstdint>

using namespace std;

namespace errors
// algoritmos para deteccao e correcao de erros
{
    // lookup table para o CRC8
    const uint16_t POLYNOMIAL_8 = 0xE7; // 111100111 -> x^8 + x^7 + x^6 + x^5 + x^2 + x^1 + 1
    static uint8_t const LOOKUP_TABLE_8[] = {
        0x00, 0xE7, 0x29, 0xCE, 0x52, 0xB5, 0x7B, 0x9C, 0xA4, 0x43, 0x8D, 0x6A, 0xF6, 0x11, 0xDF, 0x38,
        0xAF, 0x48, 0x86, 0x61, 0xFD, 0x1A, 0xD4, 0x33, 0x0B, 0xEC, 0x22, 0xC5, 0x59, 0xBE, 0x70, 0x97,
        0xB9, 0x5E, 0x90, 0x77, 0xEB, 0x0C, 0xC2, 0x25, 0x1D, 0xFA, 0x34, 0xD3, 0x4F, 0xA8, 0x66, 0x81,
        0x16, 0xF1, 0x3F, 0xD8, 0x44, 0xA3, 0x6D, 0x8A, 0xB2, 0x55, 0x9B, 0x7C, 0xE0, 0x07, 0xC9, 0x2E,
        0x95, 0x72, 0xBC, 0x5B, 0xC7, 0x20, 0xEE, 0x09, 0x31, 0xD6, 0x18, 0xFF, 0x63, 0x84, 0x4A, 0xAD,
        0x3A, 0xDD, 0x13, 0xF4, 0x68, 0x8F, 0x41, 0xA6, 0x9E, 0x79, 0xB7, 0x50, 0xCC, 0x2B, 0xE5, 0x02,
        0x2C, 0xCB, 0x05, 0xE2, 0x7E, 0x99, 0x57, 0xB0, 0x88, 0x6F, 0xA1, 0x46, 0xDA, 0x3D, 0xF3, 0x14,
        0x83, 0x64, 0xAA, 0x4D, 0xD1, 0x36, 0xF8, 0x1F, 0x27, 0xC0, 0x0E, 0xE9, 0x75, 0x92, 0x5C, 0xBB,
        0xCD, 0x2A, 0xE4, 0x03, 0x9F, 0x78, 0xB6, 0x51, 0x69, 0x8E, 0x40, 0xA7, 0x3B, 0xDC, 0x12, 0xF5,
        0x62, 0x85, 0x4B, 0xAC, 0x30, 0xD7, 0x19, 0xFE, 0xC6, 0x21, 0xEF, 0x08, 0x94, 0x73, 0xBD, 0x5A,
        0x74, 0x93, 0x5D, 0xBA, 0x26, 0xC1, 0x0F, 0xE8, 0xD0, 0x37, 0xF9, 0x1E, 0x82, 0x65, 0xAB, 0x4C,
        0xDB, 0x3C, 0xF2, 0x15, 0x89, 0x6E, 0xA0, 0x47, 0x7F, 0x98, 0x56, 0xB1, 0x2D, 0xCA, 0x04, 0xE3,
        0x58, 0xBF, 0x71, 0x96, 0x0A, 0xED, 0x23, 0xC4, 0xFC, 0x1B, 0xD5, 0x32, 0xAE, 0x49, 0x87, 0x60,
        0xF7, 0x10, 0xDE, 0x39, 0xA5, 0x42, 0x8C, 0x6B, 0x53, 0xB4, 0x7A, 0x9D, 0x01, 0xE6, 0x28, 0xCF,
        0xE1, 0x06, 0xC8, 0x2F, 0xB3, 0x54, 0x9A, 0x7D, 0x45, 0xA2, 0x6C, 0x8B, 0x17, 0xF0, 0x3E, 0xD9,
        0x4E, 0xA9, 0x67, 0x80, 0x1C, 0xFB, 0x35, 0xD2, 0xEA, 0x0D, 0xC3, 0x24, 0xB8, 0x5F, 0x91, 0x76};

    // lookup table para o CRC16
    const uint16_t POLYNOMIAL_16 = 0xD175; // 1101000101110101 -> x^16 + x^15 + x^14 + x^12 + x^8 + x^6 + x^5 + x^4 + x^2 + 1
    static uint16_t const LOOKUP_TABLE_16[] = {
        0x0000, 0xD175, 0x739F, 0xA2EA, 0xE73E, 0x364B, 0x94A1, 0x45D4, 0x1F09, 0xCE7C, 0x6C96, 0xBDE3, 0xF837, 0x2942, 0x8BA8, 0x5ADD,
        0x3E12, 0xEF67, 0x4D8D, 0x9CF8, 0xD92C, 0x0859, 0xAAB3, 0x7BC6, 0x211B, 0xF06E, 0x5284, 0x83F1, 0xC625, 0x1750, 0xB5BA, 0x64CF,
        0x7C24, 0xAD51, 0x0FBB, 0xDECE, 0x9B1A, 0x4A6F, 0xE885, 0x39F0, 0x632D, 0xB258, 0x10B2, 0xC1C7, 0x8413, 0x5566, 0xF78C, 0x26F9,
        0x4236, 0x9343, 0x31A9, 0xE0DC, 0xA508, 0x747D, 0xD697, 0x07E2, 0x5D3F, 0x8C4A, 0x2EA0, 0xFFD5, 0xBA01, 0x6B74, 0xC99E, 0x18EB,
        0xF848, 0x293D, 0x8BD7, 0x5AA2, 0x1F76, 0xCE03, 0x6CE9, 0xBD9C, 0xE741, 0x3634, 0x94DE, 0x45AB, 0x007F, 0xD10A, 0x73E0, 0xA295,
        0xC65A, 0x172F, 0xB5C5, 0x64B0, 0x2164, 0xF011, 0x52FB, 0x838E, 0xD953, 0x0826, 0xAACC, 0x7BB9, 0x3E6D, 0xEF18, 0x4DF2, 0x9C87,
        0x846C, 0x5519, 0xF7F3, 0x2686, 0x6352, 0xB227, 0x10CD, 0xC1B8, 0x9B65, 0x4A10, 0xE8FA, 0x398F, 0x7C5B, 0xAD2E, 0x0FC4, 0xDEB1,
        0xBA7E, 0x6B0B, 0xC9E1, 0x1894, 0x5D40, 0x8C35, 0x2EDF, 0xFFAA, 0xA577, 0x7402, 0xD6E8, 0x079D, 0x4249, 0x933C, 0x31D6, 0xE0A3,
        0x21E5, 0xF090, 0x527A, 0x830F, 0xC6DB, 0x17AE, 0xB544, 0x6431, 0x3EEC, 0xEF99, 0x4D73, 0x9C06, 0xD9D2, 0x08A7, 0xAA4D, 0x7B38,
        0x1FF7, 0xCE82, 0x6C68, 0xBD1D, 0xF8C9, 0x29BC, 0x8B56, 0x5A23, 0x00FE, 0xD18B, 0x7361, 0xA214, 0xE7C0, 0x36B5, 0x945F, 0x452A,
        0x5DC1, 0x8CB4, 0x2E5E, 0xFF2B, 0xBAFF, 0x6B8A, 0xC960, 0x1815, 0x42C8, 0x93BD, 0x3157, 0xE022, 0xA5F6, 0x7483, 0xD669, 0x071C,
        0x63D3, 0xB2A6, 0x104C, 0xC139, 0x84ED, 0x5598, 0xF772, 0x2607, 0x7CDA, 0xADAF, 0x0F45, 0xDE30, 0x9BE4, 0x4A91, 0xE87B, 0x390E,
        0xD9AD, 0x08D8, 0xAA32, 0x7B47, 0x3E93, 0xEFE6, 0x4D0C, 0x9C79, 0xC6A4, 0x17D1, 0xB53B, 0x644E, 0x219A, 0xF0EF, 0x5205, 0x8370,
        0xE7BF, 0x36CA, 0x9420, 0x4555, 0x0081, 0xD1F4, 0x731E, 0xA26B, 0xF8B6, 0x29C3, 0x8B29, 0x5A5C, 0x1F88, 0xCEFD, 0x6C17, 0xBD62,
        0xA589, 0x74FC, 0xD616, 0x0763, 0x42B7, 0x93C2, 0x3128, 0xE05D, 0xBA80, 0x6BF5, 0xC91F, 0x186A, 0x5DBE, 0x8CCB, 0x2E21, 0xFF54,
        0x9B9B, 0x4AEE, 0xE804, 0x3971, 0x7CA5, 0xADD0, 0x0F3A, 0xDE4F, 0x8492, 0x55E7, 0xF70D, 0x2678, 0x63AC, 0xB2D9, 0x1033, 0xC146};

    uint8_t checksum8(char *data, size_t size);
    uint16_t checksum16(char *data, size_t size);

    uint8_t crc8(char *data, size_t size);
    uint8_t crc8slow(char *data, size_t size);
    uint16_t crc16(char *data, size_t size);
    uint16_t crc16slow(char *data, size_t size);
    void genCRCtable(uint8_t polynomial);
    void genCRCtable(uint16_t polynomial);

    char *convolutionalCode(char *data, size_t size);
    char *viterbiDecoder(char *data, size_t size);
} // namespace erros

#endif
