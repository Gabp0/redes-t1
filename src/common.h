#ifndef __COMMON__
#define __COMMON__

#include <fstream>
#include <iostream>
#include <string>

using namespace std;

namespace common{

extern fstream log;

void initLog(string file);
}

#endif