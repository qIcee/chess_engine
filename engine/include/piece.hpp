#pragma once
#include <cstdint>

enum Piece : uint8_t {
    EMPTY = 0,
    WP = 1, WN, WB, WR, WQ, WK,
    BP = 9, BN, BB, BR, BQ, BK
};

inline bool is_white(Piece piece) {
    return piece >= WP && piece <= WK;
}

inline bool is_black(Piece piece) {
    return piece >= BP && piece <= BK;
}