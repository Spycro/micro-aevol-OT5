#include "CustomBitset.h"

int CustomBitset::length() const {
    return 5000;
}

void CustomBitset::set(int pos, bool c){
    int bytePos = pos /8;
    int internalPos = pos %8;
    char t = 1<<internalPos;
    internalSet[bytePos] |= t;
}