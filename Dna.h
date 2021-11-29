//
// Created by arrouan on 01/10/18.
//

#pragma once

#include <vector>
#include <bitset>
#include <zlib.h>

#include "Threefry.h"
#include "aevol_constants.h"
#include "CustomBitset.h"

class Dna {

public:
    Dna() = default;

    Dna(const Dna &clone) = default;

    Dna(int length, Threefry::Gen &&rng);

    Dna& operator=(const Dna& other);

    ~Dna() = default;

    int length() const;

    void save(uint8_t *buffer) const;

    unsigned int getSaveSize() const;

    void load(gzFile backup_file);

    void set(int pos, char c);


    void do_switch(int pos);


    int promoter_at(int pos);
    int promoter_at_shift(int pos);

    bool terminator_at(int pos);
    bool terminator_at_shift(int pos);

    bool shine_dal_start(int pos);
    bool shine_dal_start_shift(int pos);

    bool protein_stop(int pos);

    int codon_at(int pos);

    CustomBitset seq_;
};
