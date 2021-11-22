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

    uint32_t getAround(int pos, int length) const;

    unsigned char internalSet[5000];
};
