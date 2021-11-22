#include "CustomBitset.h"

int CustomBitset::length() const {
    return 5000;
}

void CustomBitset::set(int pos, bool c){
    int bytePos = pos /8;
    int internalPos = pos %8;
    char t = '\x01'<<internalPos;
    internalSet[bytePos] |= (-c^internalSet[bytePos])&t;
}

void CustomBitset::flip(int pos){
    int bytePos = pos /8;
    int internalPos = pos %8;
    char t = '\x01'<<internalPos;
    internalSet[bytePos] ^= t;
}

bool CustomBitset::get(int pos) const{
    int bytePos = pos /8;
    int internalPos = pos %8;

    return (internalSet[bytePos] >> internalPos) & '\x01';
}