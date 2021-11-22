#pragma once

class CustomBitset {

public:
    CustomBitset();

    CustomBitset(const CustomBitset &clone) = default;

    ~CustomBitset() = default;

    int size() const;

    void set(int pos, bool c);

    void flip(int pos);

    bool get(int pos) const;

    char internalSet[5000];
};
