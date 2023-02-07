#include "common.h"

fstream common::lout;

void common::initLog(string file){
    lout = fstream{file, lout.binary | lout.trunc | lout.in | lout.out};
    if (!lout.is_open())
        std::cout << "failed to open " << file << '\n';
    return;
}