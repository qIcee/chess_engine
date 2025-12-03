#pragma once
#include <cstdint>

enum DirectionOffsets: int8_t {
    WHITE_FORWARD = 8,
    BLACK_FORWARD = -8,

    WHITE_LEFT_CAPTURE = 7,
    WHITE_RIGHT_CAPTURE = 9,
    BLACK_LEFT_CAPTURE = -7,
    BLACK_RIGHT_CAPTURE = -9,

    STRAIGHT_UP = 8,
    STRAIGHT_DOWN = -8,
    STRAIGHT_LEFT = -1,
    STRAIGHT_RIGHT = 1,
    
    DIAG_UP_LEFT = 7,
    DIAG_UP_RIGHT = 9,
    DIAG_DOWN_LEFT = -7,
    DIAG_DOWN_RIGHT = -9,

    KNIGHT_UP_LEFT = 16-1,
    KNIGHT_UP_RIGHT = 16+1,
    KNIGHT_RIGHT_UP = 2+8,
    KNIGHT_RIGHT_DOWN = 2-8,
    KNIGHT_LEFT_UP = -2+8,
    KNIGHT_LEFT_DOWN = -2-8,
    KNIGHT_DOWN_RIGHT = -16+1,
    KNIGHT_DOWN_LEFT = -16-1,
};

inline int to_square(int rank, int file) {
    return rank * 8 + file;
}

inline int to_rank(int idx) {
    return idx / 8;
}

inline int to_file(int idx) {
    return idx % 8;
}

