#include "CustomBitset.h"
#include <iostream>
#include <bitset>
#include <cstring>

CustomBitset::CustomBitset(int size) : largeSet(size){
    length = size;
    chunkSize = sizeof(uint32_t)*8;
    numberOfChunks = length/chunkSize +1;
    internalSet = std::vector<uint32_t>(numberOfChunks);
}

const unsigned int& CustomBitset::size() const {
    return length;
}

void CustomBitset::set(int pos, bool c){
    int bytePos = pos /chunkSize;
    int internalPos =pos % chunkSize;
    internalSet[bytePos] |= (-c^internalSet[bytePos])&(1ul<<internalPos);

    largeSet[pos] = c;
}

void CustomBitset::flip(int pos){
    int bytePos = pos / chunkSize;
    int internalPos = pos % chunkSize;
    internalSet[bytePos] ^= (1ul<<internalPos);

    largeSet[pos] = !largeSet[pos];
}

const uint8_t& CustomBitset::get(const int&  pos) const{
    /*int bytePos = pos / chunkSize;
    int internalPos = pos % chunkSize;
    return (internalSet[bytePos] >> internalPos) & 1ul;*/
    return largeSet[pos];
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
        int byteLength =  32 /chunkSize  ;

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