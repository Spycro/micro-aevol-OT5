#include "CustomBitset.h"
#include <iostream>
#include <bitset>
#include <cstring>

constexpr unsigned char evo = 0b10000000;
CustomBitset::CustomBitset(){
    chunkSize = sizeof(internalSet[0])*8;
    for(int i = 0; i < numberOfChunks;++i){
        internalSet[i] = 0ul;
    }
}

const unsigned int& CustomBitset::size() const {
    return length;
}

void CustomBitset::set(int pos, bool c){
    int bytePos = pos /chunkSize;
    int internalPos =pos % chunkSize;
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
    if(false){//slightly faster but requires machine to be little endian
        int bytePos = pos / chunkSize;
        int internalPos =pos % chunkSize;
        unsigned long long res = 0;
        memcpy(((void*)&res),(void*)&internalSet[bytePos],8);
        res =res>>internalPos;        
        return res;
    }else{
        int bytePos = pos / chunkSize;
        int internalPos =pos % chunkSize;
        int byteLength =  std::max(32 /chunkSize,1)  ;

        unsigned long long res = 0;
        #pragma omp simd
        for(int i = 0; i<= byteLength;++i){
            unsigned long long t = (internalSet[bytePos + i]);
            t = t<< (i*chunkSize);
            res |=t;
        }
        res = res >> internalPos;
        return res ;
    }
    
}