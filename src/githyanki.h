#ifndef __PRTC__
#define __PRTC__

#include <string>
#include <bitset>

using namespace std;

// Nao pode dar include no Connection.h por causa de include ciclico entao fazemos apenas a assinatura do Connection
class Connection;
// Global Intelligent Technology Handling Yielding Advanced Network Knowledge Interface Protocol
namespace Githyanki
{
  // Chance to not receive frame
  static const short chanceLostFrame = 0;

  // Sizes
  static const short FRAME_SIZE_MAX = 256;
  static const short CHECK_SIZE = 2;
  static const short HEADER_SIZE = 2;
  static const short WINDOW_MAX = 16;
  static const short SEND_WINDOW_MAX = WINDOW_MAX / 2;
  static const short DATA_SIZE_MAX = FRAME_SIZE_MAX - CHECK_SIZE - HEADER_SIZE;
  static const short RECIEVE_DATABUFFER_MAX = 100;
  static const short MINIMUM_FRAME_SIZE = 36;

  // MSG
  static const short SUCESS = 200;
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
  static const short TEXT = 0x1;    // 1
  static const short FILE = 0x2;    // 2
  static const short ERROR = 0x9;   // 9
  static const short ACK = 0xA;     // 10
  static const short NACK = 0xB;    // 11
  static const short TIMEOUT = 0xD; // 13
  static const short INIT = 0xE;    // 14
  static const short END = 0xF;     // 15

  static const short VALID_TYPES[] = {TEXT, FILE, ACK, NACK, ERROR, INIT, END, TIMEOUT};

  struct Ack
  {
    // Ack or Nack
    unsigned short type;
    unsigned short seq;

    Ack(){};
    Ack(unsigned short t, unsigned short s) : type(t), seq(s){};
  };

  struct Frame
  {
    unsigned short type : 4;        // Meio byte
    unsigned short seq : 4;         // Meio byte
    unsigned short sizeData : 8;    // Um byte
    char *data;
    char checksum[CHECK_SIZE];

    ~Frame();
    Frame();
    Frame(unsigned short type, unsigned short seq);
    Frame(const char *data, size_t sizeData, unsigned short type, unsigned short seq);
    void toString();
    size_t toBytes(char *buffer);
    void fromBytes(void *bytes);
    int checkError();
    void calcError();
  };

  struct DataObject
  {
    char *data;

    int bytesFramed;
    long size;

    char *name;
    int nameSize;

    short type;
    int frameQty;
    Connection *myCon;
    Connection *otherCon;
    std::FILE *file;

    ~DataObject();
    DataObject();
    DataObject(char *data);
    DataObject(char *data, char *name);
    char* getBytes(int size);
  };

  struct Window
  {
    int window[SEND_WINDOW_MAX];
    Frame *frames[SEND_WINDOW_MAX];

    int lastSeq; // Next sequence thats not in use
    int firstNotFramedIndex;
    int endSeq;
    int sendingFrames;

    void prepareFrames(Githyanki::DataObject *obj);
    void acknowledge(Ack *ack);
    void init();
  };

  struct place
  {
    int seq;
    int posi;
    bool received;

    place(int s, int p) : seq(s), posi(p) { received = false; };
  };

  struct DataBlock
  {
    char *data;
    int size;

    ~DataBlock();
    DataBlock(char *data, int size);
  };

  struct WindowRec
  {
    place *windowPlace[SEND_WINDOW_MAX];
    DataBlock *windowData[RECIEVE_DATABUFFER_MAX + 1];

    DataObject *obj;

    int lastAck;
    int windowDataSize;
    int firstSeq;
    int lastSeq; // Next sequence thats not in use
    int lastDataIndex;
    int endSeq;
    bool finishedAcked;
    int receivedFrames;

    void finalize(Frame *frame);
    void bufferFrame(Frame *frame);
    void acknowledge();
    void init();
  };

  int sendText();
  int establishConnection(Connection *con);
  int SlidingWindowSend(Githyanki::DataObject *obj);
  Githyanki::DataObject *SlidingWindowReceive(Connection *myCon, Connection *otherCon);
  void printFrame(Githyanki::Frame *f);
  unsigned short checksum(unsigned short *buff, int _16bitword);
  int isValid(char *buffer, int tamanho_buffer, Frame *frame);
};

#endif // !__PRTC__
