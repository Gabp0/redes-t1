#ifndef __COMMON__
#define __COMMON__

#include <fstream>
#include <iostream>
#include <string>
#include "githyanki.h"

using namespace std;

template<typename T> void safe_delete(T*& a) {
  delete a;
  a = NULL;
}

namespace common{

extern fstream lout;
extern fstream fout;

void initLog(string file);
void closeLog();

bool randomChance(int y);
int randomBetween(int x, int y);
void randomSeed();

int distWindow(int f, int l, int s);
bool ackIf(int f, int l, int s, int c);

void initFile(string file);
void closeFile();
void flushBuffer(Githyanki::DataBlock **buffer, int size);
}

#endif