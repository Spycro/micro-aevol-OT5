#pragma once
#include <stdint.h>
class CustomBitset {

public:
    CustomBitset();

    CustomBitset(const CustomBitset &clone) = default;

    ~CustomBitset() = default;

    int size() const;

    void set(int pos, bool c);

    void flip(int pos);

    bool get(int pos) const;

    uint32_t getAround(int pos) const;

private:
    unsigned int length = 5000;
    uint8_t internalSet[5000];
    int chunkSize;
    int numberOfChunks = 5000;
};
