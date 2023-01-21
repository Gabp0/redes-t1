#ifndef CHECKSUM
#define CHECKSUM

#include <bits/stdc++.h>
#include <string.h>
using namespace std;

string ones_complement(string data);
string checkSum(string data, int block_size);
bool checker(string sent_message, string rec_message, int block_size);

#endif // !CHECKSUM
