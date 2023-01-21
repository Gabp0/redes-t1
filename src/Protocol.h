#ifndef __PRTC__
#define __PRTC__

#include <string>

using namespace std;

class Protocol
{
public:
  static const short SUCESS = 200;
  static const short TIMEOUT = 201;
  static const short NO_CONECTION = 202;
  static const short INV_CMD = 203;
  static const short FAILURE = 204;
  static const short FILE_NF = 205;
  static const short FILE_OE = 206;
  static const short FILE_WE = 207;
  static const short CONECTION_REST = 208;

  static const short MARK = 0x01;

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
    unsigned short mark : 8;
    unsigned short type : 6;
    unsigned short seq : 4;
    unsigned short size : 6;
    // string message;
    unsigned short checksum : 16;
  };

  Protocol(string device);
  int receiveMessage(int timeoutMillis, char *buffer, int tamanho_buffer);
  void sendMessage(string message, char seq);
  //~Protocol();
  frame *createFrame(string message, unsigned short type, unsigned short seq);

private:
  int socket;
  unsigned short checksum(unsigned short *buff, int _16bitword);
  long long timestamp();
  int isValid(char *buffer, int tamanho_buffer);
};

#endif // !__PRTC__
