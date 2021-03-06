//
// Created by elturpin on 04/12/2020.
//

#pragma once
#define SOH \x01
#include <cstdint>

constexpr int8_t NB_BASE = 2;
constexpr int8_t CODON_SIZE = 3;
constexpr uint8_t SECTION_SIZE = 32;
// promoter
constexpr int8_t PROM_MAX_DIFF = 4;
constexpr int8_t PROM_SIZE = 22;
//constexpr const char *PROM_SEQ = "\0\x01\0\x01\0\x01\x01\0\0\x01\x01\x01\0\0\x01\0\0\x01\0\x01\x01\0";//0101011001110010010110
const std::bitset<22> PROM_SEQ{"0110100100111001101010"};//0110100100111001101010
constexpr uint32_t PROM_SEQ_INT = 0b0110100100111001101010;
constexpr uint32_t PROM_SEQ_MASK_INT = 0b1111111111111111111111;
// terminator
constexpr int8_t TERM_STEM_SIZE = 4;
constexpr int8_t TERM_LOOP_SIZE = 3;
constexpr int8_t TERM_SIZE = TERM_STEM_SIZE + TERM_LOOP_SIZE + TERM_STEM_SIZE;
extern bool TERMINATOR_LOOKUP_TABLE[];
constexpr uint32_t TERM_MASK_INT = 0b11110001111;
// shine dalgardo
constexpr int8_t SHINE_DAL_SIZE = 6;
constexpr int8_t SD_START_SPACER = 4;
constexpr int8_t SD_TO_START = SHINE_DAL_SIZE + SD_START_SPACER + CODON_SIZE;
//constexpr const char *SHINE_DAL_SEQ = "\0\x01\x01\0\x01\x01xxxx\0\0\0";//"011011****000";
const std::bitset<13> SHINE_DAL_SEQ {"0000000110110"};//0000000110110
constexpr uint32_t SHINE_DAL_SEQ_INT = 0b0000000110110;
const std::bitset<13> SHINE_DAL_MASK{"1110000111111"};
constexpr uint32_t SHINE_DAL_MASK_INT = 0b1110000111111;
// stop
//constexpr const char *PROTEIN_END = "\0\0\x01";//"001"; // CODON_STOP
const std::bitset<3> PROTEIN_END{"100"};//100

constexpr int32_t DO_TRANSLATION_LOOP = SHINE_DAL_SIZE + SD_START_SPACER + 3 * CODON_SIZE;

// Codon
constexpr int8_t CODON_START = 0b000;
constexpr int8_t CODON_STOP  = 0b001;
constexpr int8_t CODON_M0    = 0b100;
constexpr int8_t CODON_M1    = 0b101;
constexpr int8_t CODON_W0    = 0b010;
constexpr int8_t CODON_W1    = 0b011;
constexpr int8_t CODON_H0    = 0b110;
constexpr int8_t CODON_H1    = 0b111;

// Protein / Fuzzy space
constexpr double X_MIN = 0.0;
constexpr double X_MAX = 1.0;
constexpr double Y_MIN = 0.0;
constexpr double Y_MAX = 1.0;
constexpr double H_MIN = -1.0;
constexpr double H_MAX = 1.0;
constexpr double W_MIN = 0.0;
constexpr double W_MAX = 0.1;

constexpr int FUZZY_SAMPLING = 300;
constexpr int SELECTION_PRESSURE = 1000;

// Selection
constexpr int8_t NEIGHBORHOOD_WIDTH  = 3;
constexpr int8_t NEIGHBORHOOD_HEIGHT = 3;
constexpr int8_t NEIGHBORHOOD_SIZE   = NEIGHBORHOOD_HEIGHT * NEIGHBORHOOD_WIDTH;
