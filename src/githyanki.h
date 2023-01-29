#ifndef __PRTC__
#define __PRTC__

#include <string>
#include <bitset>



using namespace std;

//Nao pode dar include no Connection.h por causa de include ciclico entao fazemos apenas a assinatura do Connection
class Connection;
// Global Intelligent Technology Handling Yielding Advanced Network Knowledge Interface Protocol
namespace Githyanki
{
  // Sizes
  static const short FRAME_SIZE_MAX = 256;
  static const short DATA_SIZE_MAX = 253;
  static const short CHECK_SIZE = 1;
  static const short WINDOW_MAX = 16;
  static const short SEND_WINDOW_MAX = WINDOW_MAX/2;

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

  struct Ack{
    //Ack or Nack
    unsigned short type;
    unsigned short seq;
  };

  struct Frame
  {
    unsigned short type : 6;
    unsigned short seq : 4;
    unsigned short sizeData : 6;
    char data[DATA_SIZE_MAX];
    char checksum[CHECK_SIZE];

    Frame();
    Frame(unsigned short type, unsigned short seq);
    Frame(const char *data, size_t data_size, unsigned short type, unsigned short seq);
    string toString();
    size_t toBytes(char *buffer);
    void fromBytes(void *bytes);
  };

  struct DataObject
  {
    char *data;

    int bytesFramed;
    int size;

    char *name;  
    int nameSize;

    short type;
    int frameQty;
    Connection *con;

    ~DataObject();
    DataObject();
    DataObject(char *data);
    DataObject(char *data, char *name);
  };

  struct Window{
    int window[SEND_WINDOW_MAX];
    Frame *frames[SEND_WINDOW_MAX];

    int lastSeq; //Next sequence thats not in use
    int firstNotFramedIndex;

    void acknowledge(Ack ack);
    void init();
  };

  int SlidingWindowSend(Githyanki::DataObject *obj);
  Githyanki::DataObject SlidingWindowReceive(Connection con);
  void printFrame(Githyanki::Frame  *f);
  unsigned short checksum(unsigned short *buff, int _16bitword);
  int isValid(char *buffer, int tamanho_buffer, Frame *frame);
};

#endif // !__PRTC__
