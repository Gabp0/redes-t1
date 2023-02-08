#ifndef __COMMON__
#define __COMMON__

#include <fstream>
#include <iostream>
#include <string>

using namespace std;

namespace common{

extern fstream lout;

void initLog(string file);
void closeLog();
bool randomChance(int y);
int randomBetween(int x, int y);
void randomSeed();
int distWindow(int f, int l, int s);
bool ackIf(int f, int l, int s, int c);
void flushBuffer(string **buffer, int size);
}

#endif