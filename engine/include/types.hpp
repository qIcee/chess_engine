#pragma once
#include <cstdint>


constexpr int NORTH = +8,
            SOUTH = -8,
            EAST = +1,
            WEST = -1,
            NORTH_EAST = +9,
            NORTH_WEST = +7,
            SOUTH_EAST = -7,
            SOUTH_WEST = -9,

            WHITE_FORWARD = NORTH,
            BLACK_FORWARD = SOUTH,
            WHITE_CAPTURE_LEFT = NORTH_WEST,
            WHITE_CAPTURE_RIGHT = NORTH_EAST,
            BLACK_CAPTURE_LEFT = SOUTH_WEST,
            BLACK_CAPTURE_RIGHT = SOUTH_EAST;
            
constexpr int KING_OFFSETS[8] = {
    NORTH, SOUTH, EAST, WEST, NORTH_EAST, NORTH_WEST, SOUTH_EAST, SOUTH_WEST
};

constexpr int KNIGHT_OFFSETS[8] = {
    +17, +15, +10, +6,
    -6, -10, -15, -17
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

