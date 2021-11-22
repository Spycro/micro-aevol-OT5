#include "CustomBitset.h"
#include <iostream>
#include <bitset>
#include <cstring>
CustomBitset::CustomBitset(){
    chunkSize = sizeof(internalSet[0])*8;
    for(int i = 0; i < size();++i){
        internalSet[i] = 0ul;
    }
}

int CustomBitset::size() const {
    return length;
}

void CustomBitset::set(int pos, bool c){
    int bytePos = pos /chunkSize;
    int internalPos = pos % chunkSize;
    internalSet[bytePos] |= (-c^internalSet[bytePos])&(1ul<<internalPos);
}

void CustomBitset::flip(int pos){
    int bytePos = pos / chunkSize;
    int internalPos = pos % chunkSize;
    internalSet[bytePos] ^= (1ul<<internalPos);
}

bool CustomBitset::get(int pos) const{
    int bytePos = pos / chunkSize;
    int internalPos = pos % chunkSize;
    return (internalSet[bytePos] >> internalPos) & 1ul;
}

uint32_t CustomBitset::getAround(int pos) const{
    int bytePos = pos / chunkSize;
    int internalPos = pos % chunkSize;
    int byteLength = 32/chunkSize +2;
    uint64_t res = 0;
    for(int i = 0; i< byteLength;++i){
        uint64_t t = internalSet[bytePos + i];
        res |= (t<<(i*chunkSize));
    }
    res = (res>>internalPos);
    return res;
}