//
// Created by arrouan on 01/10/18.
//

#include "Dna.h"
#include <omp.h>
#include <cassert>
extern double staticMTime;
Dna::Dna(int length, Threefry::Gen &&rng) {
    // Generate a random genome
    for (int32_t i = 0; i < length; i++) {
        seq_[i] = rng.random(NB_BASE);
    }
}

int Dna::length() const {
    return seq_.size();
}

void Dna::save(gzFile backup_file) {
    int dna_length = length();
    std::vector<char> translatedSeq(dna_length);
    #pragma omp simd
    for(int i = 0; i< dna_length;++i){
        translatedSeq[i] = seq_[i] + '0';
    }
    gzwrite(backup_file, &dna_length, sizeof(dna_length));
    gzwrite(backup_file, translatedSeq.data(), dna_length * sizeof(translatedSeq[0]));
}

void Dna::load(gzFile backup_file) {
    int dna_length;
    gzread(backup_file, &dna_length, sizeof(dna_length));

    char tmp_seq[dna_length];
    gzread(backup_file, tmp_seq, dna_length * sizeof(tmp_seq[0]));
    #pragma omp simd
    for(int i = 0 ; i< dna_length ;++i){
        tmp_seq[i] = tmp_seq[i]-'0';
        seq_[i] = tmp_seq[i];
    }

  //  seq_ = std::vector<char>(tmp_seq, tmp_seq + dna_length);
}

void Dna::set(int pos, char c) {
    seq_[pos] = c;
}

void Dna::do_switch(int pos) {
    seq_.flip(pos);
}



int Dna::promoter_at(int pos) {
    char prom_dist[PROM_SIZE];
    //double t = omp_get_wtime();
    if((pos + PROM_SIZE)< length()){
        double t = omp_get_wtime();
        #pragma omp simd
        for (int motif_id = 0; motif_id < PROM_SIZE; motif_id++) {
            // Searching for the promoter
            prom_dist[motif_id] =
                    PROM_SEQ[motif_id] ^ seq_[pos + motif_id];
        } 
        staticMTime += omp_get_wtime() -t;
    }else{
        
        for (int motif_id = 0; motif_id < PROM_SIZE; motif_id++) {
            int search_pos = pos + motif_id;
            if (search_pos >=length())
                search_pos -= length();
            // Searching for the promoter
            prom_dist[motif_id] =
                    PROM_SEQ[motif_id] ^ seq_[search_pos];
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

// Given a, b, c, d boolean variable and X random boolean variable,
// a terminator look like : a b c d X X !d !c !b !a
int Dna::terminator_at(int pos) {
    if(true){
        int left[4];
        int right[4];
        if(pos + TERM_SIZE < length()){
            for(int i = 0;i<TERM_STEM_SIZE;++i){
                left[i] = pos +i;
                right[i] = pos + (TERM_SIZE -1) -i;
                if(seq_[left[i]] == seq_[right[i]]){
                    return 0;
                }
            }
            return TERM_STEM_SIZE;
        }else{
            for(int i = 0;i<TERM_STEM_SIZE;++i){
                left[i] = pos +i;
                left[i] = left[i] >= length() ? left[i] - length() : left[i];
                right[i] = pos + (TERM_SIZE -1) -i;
                right[i] = right[i] >= length() ? right[i] - length() : right[i];
                if(seq_[left[i]] == seq_[right[i]]){
                    return 0;
                }
            }
            return TERM_STEM_SIZE;
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
            term_dist[motif_id] = !(seq_[right] ^ seq_[left]);
        }
        int dist_term_lead = term_dist[0] +
                            term_dist[1] +
                            term_dist[2] +
                            term_dist[3];
        return dist_term_lead;
    }

}

bool Dna::shine_dal_start(int pos) {
    bool start = false;
    int t_pos, k_t;

    for (int k = 0; k < SHINE_DAL_SIZE + CODON_SIZE; k++) {
        k_t = k >= SHINE_DAL_SIZE ? k + SD_START_SPACER : k;
        t_pos = pos + k_t;
        if (t_pos >= seq_.size())
            t_pos -= seq_.size();

        if (seq_[t_pos] == SHINE_DAL_SEQ[k_t]) {
            start = true;
        } else {
            start = false;
            break;
        }
    }

    return start;
}

bool Dna::protein_stop(int pos) {
    bool is_protein;
    int t_k;

    for (int k = 0; k < CODON_SIZE; k++) {
        t_k = pos + k;
        if (t_k >= seq_.size())
            t_k -= seq_.size();

        if (seq_[t_k] == PROTEIN_END[k]) {
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
        if (seq_[t_pos] == '\x01')
            value += 1 << (CODON_SIZE - i - 1);
    }

    return value;
}