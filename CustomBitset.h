#pragma once
#include <stdint.h>
class CustomBitset {

public:
    CustomBitset();

    CustomBitset(const CustomBitset &clone) = default;

    ~CustomBitset() = default;

    const unsigned int& size() const;

    void set(int pos, bool c);

    void flip(int pos);

    bool get(int pos) const;

    uint32_t getAround(int pos)const ;

private:
    unsigned int length = 5000;
    int numberOfChunks = 157;
    uint32_t internalSet[157];
    int chunkSize;
    
};
