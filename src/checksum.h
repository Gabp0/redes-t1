#ifndef __CHKS__
#define __CHKS__

#include <bits/stdc++.h>
#include <string.h>
using namespace std;

string calcCheckSum(string data, int block_size = 8);
bool checker(string rec_message, string rec_checksum, int block_size = 8);

#endif // !CHECKSUM
