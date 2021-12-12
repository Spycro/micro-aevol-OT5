#pragma once
#include <stdint.h>
#include <vector>
class CustomBitset {

public:
    CustomBitset() = default;

    CustomBitset(int size);

    CustomBitset(const CustomBitset &clone) = default;

    CustomBitset& operator=(const CustomBitset& other);

    ~CustomBitset() = default;

    const unsigned int& size() const;

    void set(int pos, bool c);

    void flip(int pos);

    const uint8_t& get(const int& pos) const;

    const uint32_t& getAround(const int& pos) ;

    const uint32_t& getAroundShift(const int& pos);
private:
    unsigned int length;
    int numberOfChunks;
    std::vector<uint32_t> internalSet;
    std::vector<uint8_t> largeSet;
    int chunkSize;

    uint32_t sectionCache;
    int posCache =-42;
    bool bitsChanged= true;
};
