#include "common.h"

fstream common::log;

void common::initLog(string file){
    log = fstream{file, log.binary | log.trunc | log.in | log.out};
    if (!log.is_open())
        std::cout << "failed to open " << file << '\n';
    return;
}