#include "CustomBitset.h"
#include <iostream>
#include <bitset>
#include <cstring>
CustomBitset::CustomBitset(){
    for(int i = 0; i < size();++i){
        internalSet[i] = '\0';
    }
}

int CustomBitset::size() const {
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
    char t = ('\x01'<<internalPos);
    internalSet[bytePos] ^= t;
}

bool CustomBitset::get(int pos) const{
    int bytePos = pos /8;
    int internalPos = pos % 8;
    return (internalSet[bytePos] >> internalPos) & '\x01';
}

uint32_t CustomBitset::getAround(int pos, int length) const{
    int bytePos = pos /8;
    int internalPos = pos % 8;
    int byteLength = 4;
    uint32_t res = 0;
    //std::memcpy(&res,(internalSet + bytePos),byteLength);
    for(int i = 0; i< byteLength;++i){
        uint32_t t = internalSet[bytePos + i];
        res |= (t<<(i*8));
    }
    res = (res>>internalPos);
    return res;
}