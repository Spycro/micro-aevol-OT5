#pragma once

class CustomBitset {

public:
    CustomBitset() = default;

    CustomBitset(const CustomBitset &clone) = default;

    ~CustomBitset() = default;

    int length() const;

    void set(int pos, bool c);

    void flip(int pos);

    bool get(int pos);

    char internalSet[5000];
};
