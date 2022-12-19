#ifndef __PRTC__
#define __PRTC__

#include <string>

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

  Protocol();
  string 
  ~Protocol();

private:
  
};


#endif // !__PRTC__
