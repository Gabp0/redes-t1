#ifndef __COMMON__
#define __COMMON__

#include <fstream>
#include <iostream>
#include <string>
#include "githyanki.h"

using namespace std;

template <typename T>
void safe_delete(T *&a)
{
  if (a == NULL)
    delete a;
  a = NULL;
}

namespace common
{
  extern fstream lout;
  extern fstream fout;
  extern std::FILE *fin;
  extern std::FILE *foutBinary;

  void initLog(string file);
  void closeLog();

  bool randomChance(int y);
  int randomBetween(int x, int y);
  void randomSeed();

  int distWindow(int f, int l, int s);
  bool ackIf(int f, int l, int s, int c);

  void initOutputFile(string file);
  void closeOutputFile();

  long initInputFile(string filePath);
  void closeInputFile();

  // void flushBuffer(Githyanki::DataBlock **buffer, int size);
}

#endif