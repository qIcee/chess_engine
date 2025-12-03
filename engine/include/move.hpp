#pragma once
#include <cstdint>

/*  
    Bits:   [15 14 13 12]   [11 ... 6]      [5 ... 0]
    Meaning:   type           to-square       from-square
    Size:       4 bits          6 bits         6 bits
*/

enum MoveType : uint8_t {
    QUIET = 0,
    DOUBLE_PUSH = 1,
    KING_CASTLE = 2,
    QUEEN_CASTLE = 3,
    CAPTURE = 4,
    EN_PASSANT = 5,

    KNIGHT_PROMO = 8,
    BISHOP_PROMO = 9,
    ROOK_PROMO = 10,
    QUEEN_PROMO = 11,

    KNIGHT_PROMO_CAPTURE = 12,
    BISHOP_PROMO_CAPTURE = 13,
    ROOK_PROMO_CAPTURE = 14,
    QUEEN_PROMO_CAPTURE = 15
};

// Converts int to binary and shifts them to their position
inline uint16_t make_move(int from, int to, int type) {
    return (from & 0x3F) 
        | ((to & 0x3F) << 6)
        | ((type & 0xF) << 12);
}

inline int from_sq(uint16_t m) {
    return m & 0x3F;
}

inline int to_sq(uint16_t m) {
    return (m >> 6) & 0x3F;
}

inline int move_type(uint16_t m) {
    return (m >> 12) & 0xF;
}