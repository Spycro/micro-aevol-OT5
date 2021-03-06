//
// Created by arrouan on 01/10/18.
//

#include "Dna.h"
#include <omp.h>
#include <cassert>
#include <cstring>
extern long long staticMTime;
Dna::Dna(int length, Threefry::Gen &&rng) : seq_(length) {
    // Generate a random genome
    for (int32_t i = 0; i < length; i++) {
        auto a = rng.random(NB_BASE);
        seq_.set(i,a);
    }
}

Dna& Dna::operator=(const Dna& other){
    seq_ = other.seq_;
    return *this;
}

int Dna::length() const {
    return seq_.size();
}

void Dna::save(uint8_t * buffer) const{
    int dna_length = length();
    memcpy(buffer,&dna_length,sizeof(dna_length));
    uint8_t * bitBuffer = buffer + sizeof(dna_length);
    #pragma omp simd
    for(int i = 0; i< dna_length;++i){
        bitBuffer[i] = seq_.get(i) + '0';
    }
}

unsigned int Dna::getSaveSize()const{
    return sizeof(int) + length();
}

void Dna::load(gzFile backup_file) {
    int dna_length;
    gzread(backup_file, &dna_length, sizeof(dna_length));

    char tmp_seq[dna_length];
    gzread(backup_file, tmp_seq, dna_length * sizeof(tmp_seq[0]));
    #pragma omp simd
    for(int i = 0 ; i< dna_length ;++i){
        tmp_seq[i] = tmp_seq[i]-'0';
        seq_.set(i,tmp_seq[i]);
    }
}

void Dna::set(int pos, char c) {
    seq_.set(pos,c);
}

void Dna::do_switch(int pos) {
    seq_.flip(pos);
}



int Dna::promoter_at(int pos) {
    char prom_dist[PROM_SIZE];
    
    if((pos + SECTION_SIZE)< length()){
        if(false){
            #pragma omp simd
            for (int motif_id = 0; motif_id < PROM_SIZE; motif_id++) {
                // Searching for the promoter
                prom_dist[motif_id] =
                        PROM_SEQ[motif_id] != seq_.get(pos + motif_id);
            } 
        }else{
            int mres;
            mres=std::bitset<22>{seq_.getAround(pos) ^ PROM_SEQ_INT}.count();
            
            return mres;
        }
        
    }else{
        
        for (int motif_id = 0; motif_id < PROM_SIZE; motif_id++) {
            int search_pos = pos + motif_id;
            if (search_pos >=length())
                search_pos -= length();
            // Searching for the promoter
            prom_dist[motif_id] =
                    PROM_SEQ[motif_id] != seq_.get(search_pos);
        }
        
    }
    //vectTime += omp_get_wtime() - t;
    
    // Computing if a promoter exists at that position
    int dist_lead =0;
    for(int pDist:prom_dist){
        dist_lead += pDist;
        if(dist_lead > PROM_MAX_DIFF){
            return dist_lead;
        }
    }
    return dist_lead;
}
int Dna::promoter_at_shift(int pos) { 
    return  std::bitset<22>{seq_.getAroundShift(pos) ^ PROM_SEQ_INT}.count();
}

// Given a, b, c, d boolean variable and X random boolean variable,
// a terminator look like : a b c d X X !d !c !b !a
bool Dna::terminator_at(int pos) {
    if(false){
        int left[4];
        int right[4];
        if(pos + TERM_SIZE < length()){
            for(int i = 0;i<TERM_STEM_SIZE;++i){
                left[i] = pos +i;
                right[i] = pos + (TERM_SIZE -1) -i;
                if(seq_.get(left[i]) == seq_.get(right[i])){
                    return false;
                }
            }
            return true;
        }else{
            for(int i = 0;i<TERM_STEM_SIZE;++i){
                left[i] = pos +i;
                left[i] = left[i] >= length() ? left[i] - length() : left[i];
                right[i] = pos + (TERM_SIZE -1) -i;
                right[i] = right[i] >= length() ? right[i] - length() : right[i];
                if(seq_.get(left[i]) == seq_.get(right[i])){
                    return false;
                }
            }
            return true;
        }
    }else if(true){
        if(pos + SECTION_SIZE < length()){
            return TERMINATOR_LOOKUP_TABLE[seq_.getAround(pos) & TERM_MASK_INT];
        }else{
            for(int i = 0;i<TERM_STEM_SIZE;++i){
                int left = pos +i;
                left = left >= length() ? left - length() : left;
                int right = pos + (TERM_SIZE -1) -i;
                right = right >= length() ? right - length() : right;
                if(seq_.get(left) == seq_.get(right)){
                    return false;
                }
            }
            return true;
        }
    }else{
        int term_dist[TERM_STEM_SIZE];
        for (int motif_id = 0; motif_id < TERM_STEM_SIZE; motif_id++) {
            int right = pos + motif_id;
            int left = pos + (TERM_SIZE - 1) - motif_id;

            // loop back the dna inf needed
            if (right >= length()) right -= length();
            if (left >= length()) left -= length();

            // Search for the terminators
            term_dist[motif_id] = (seq_.get(right) != seq_.get(left));
        }
        int dist_term_lead = term_dist[0] +
                            term_dist[1] +
                            term_dist[2] +
                            term_dist[3];
        return dist_term_lead == TERM_STEM_SIZE;
    }

}

bool Dna::terminator_at_shift(int pos) {
    return TERMINATOR_LOOKUP_TABLE[seq_.getAroundShift(pos) & TERM_MASK_INT];
}

bool Dna::shine_dal_start(int pos) {
    bool start = false;
    int t_pos, k_t;

    if((pos + SECTION_SIZE < length()) && true){
        return ((seq_.getAround(pos) ^ SHINE_DAL_SEQ_INT)&SHINE_DAL_MASK_INT) == 0;
    }else{
        for (int k = 0; k < SHINE_DAL_SIZE + CODON_SIZE; k++) {
            k_t = k >= SHINE_DAL_SIZE ? k + SD_START_SPACER : k;
            t_pos = pos + k_t;
            if (t_pos >= seq_.size())
                t_pos -= seq_.size();

            if (seq_.get(t_pos) == SHINE_DAL_SEQ[k_t]) {
                start = true;
            } else {
                start = false;
                break;
            }
        }
    }

    return start;
}

bool Dna::shine_dal_start_shift(int pos) {
    return ((seq_.getAroundShift(pos) ^ SHINE_DAL_SEQ_INT)&SHINE_DAL_MASK_INT) == 0;
}

bool Dna::protein_stop(int pos) {
    bool is_protein;
    int t_k;

    for (int k = 0; k < CODON_SIZE; k++) {
        t_k = pos + k;
        if (t_k >= seq_.size())
            t_k -= seq_.size();

        if (seq_.get(t_k) == PROTEIN_END[k]) {
            is_protein = true;
        } else {
            is_protein = false;
            break;
        }
    }

    return is_protein;
}

int Dna::codon_at(int pos) {
    int value = 0;

    int t_pos;

    for (int i = 0; i < CODON_SIZE; i++) {
        t_pos = pos + i;
        if (t_pos >= seq_.size())
            t_pos -= seq_.size();
        if (seq_.get(t_pos) == '\x01')
            value += 1 << (CODON_SIZE - i - 1);
    }

    return value;
}