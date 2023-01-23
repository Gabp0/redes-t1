#ifndef __PRTC__
#define __PRTC__

#include <string>
#include <bitset>

using namespace std;

// Global Intelligent Technology Handling Yielding Advanced Network Knowledge Interface Protocol
namespace Githyanki
{
  // Sizes
  static const short FRAME_SIZE_MAX = 256;
  static const short DATA_SIZE_MAX = 250;

  // MSG
  static const short SUCESS = 200;
  static const short TIMEOUT = 201;
  static const short NO_CONECTION = 202;
  static const short INV_CMD = 203;
  static const short FAILURE = 204;
  static const short FILE_NF = 205;
  static const short FILE_OE = 206;
  static const short FILE_WE = 207;
  static const short CONECTION_REST = 208;

  // Mark / Start of Frame
  static const short SOH = 0x3F;

  // Types
  static const short TEXT = 0x01;
  static const short MEDIA = 0x10;
  static const short AWK = 0x0A;
  static const short NACK = 0x00;
  static const short ERROR = 0x1E;
  static const short INIT = 0x1D;
  static const short END = 0x0F;
  static const short DATA = 0x0D;

  struct frame
  {
    unsigned short mark : 6;
    unsigned short type : 6;
    unsigned short seq : 4;
    uint8_t size;
    char data[DATA_SIZE_MAX];
    uint16_t checksum;

    size_t toBytes(char *buffer);
    void fromBytes(void *bytes);
  };

  frame *createFrame(const char *data, size_t data_size, unsigned short type, unsigned short seq);
  unsigned short checksum(unsigned short *buff, int _16bitword);
  int isValid(char *buffer, int tamanho_buffer);
};

#endif // !__PRTC__
