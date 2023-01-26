#include "checksum.h"
#include <iostream>

using namespace std;

char checksum8(char *data, unsigned int size)
{
  uint8_t sum = 0;
  
  for (size_t i = 0; i < size; i++) {
    sum += (uint8_t) data[i];
  }
  
  sum = ~sum;

  cout << sum << endl;
  
  return sum;
}

//char[2] checksum16(char *data, unsigned int size)
//{
  //uint16_t sum = 0;


  //for (size_t i = 0; i < size; i+=2) {
    //uint16_t cbyte = data[i];
    //cbyte = (cybte << 8);
    //cbyte = (cybte | data[i + 1])

    //sum += (uint16_t) data[i];

  //}
  
  //sum = ~sum;

  //cout << sum << endl;
  
  //return sum;
//}
