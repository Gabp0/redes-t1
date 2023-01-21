#ifndef __PRTC__
#define __PRTC__

#include <string>

using namespace std;

class Protocol
{
public:
  static const int SUCESS = 200;
  static const int TIMEOUT = 201;
  static const int NO_CONECTION = 202;
  static const int INV_CMD = 203;
  static const int FAILURE = 204;
  static const int FILE_NF = 205;
  static const int FILE_OE = 206;
  static const int FILE_WE = 207;
  static const int CONECTION_REST = 208;

  static const char MARK = 0x7e;

  static const char TEXT = 0x01;
  static const char MEDIA = 0x10;
  static const char AWK = 0x0A;
  static const char NACK = 0x00;
  static const char ERROR = 0x1E;
  static const char INIT = 0x1D;
  static const char END = 0x0F;
  static const char DATA = 0x0D;

  Protocol(string device);
  int receiveMessage(int timeoutMillis, char *buffer, int tamanho_buffer);
  void sendMessage(string message, char seq);
  //~Protocol();
  char *createFrame(string message, char seq);

private:
  int socket;
  unsigned short checksum(unsigned short *buff, int _16bitword);
  long long timestamp();
  int isValid(char *buffer, int tamanho_buffer);
};

#endif // !__PRTC__
