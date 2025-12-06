#pragma once
#include <cstdint>

enum Color {
    WHITE = 0,
    BLACK = 1,
    NO_COLOR = 2
};

enum Piece : uint8_t {
    EMPTY = 0,
    WP = 1, WN = 2, WB = 3, WR = 4, WQ = 5, WK = 6,
    BP = 9, BN = 10, BB = 11, BR = 12, BQ = 13, BK = 14
};

enum PieceType : uint8_t {
    NO_PIECE,
    PAWN,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
    KING
};

inline bool is_white(Piece piece) {
    return piece >= WP && piece <= WK;
}

inline bool is_black(Piece piece) {
    return piece >= BP && piece <= BK;
}

inline bool is_piece_of_side(Piece piece, Color side) {
    if (side == WHITE) return is_white(piece);
    if (side == BLACK) return is_black(piece);
    return false;
}

inline PieceType get_piece_type(Piece piece) {
    switch (piece) {
        case WP: case BP: return PAWN;
        case WN: case BN: return KNIGHT;
        case WB: case BB: return BISHOP;
        case WR: case BR: return ROOK;
        case WQ: case BQ: return QUEEN;
        case WK: case BK: return KING;
        case EMPTY:        return NO_PIECE;
        default:           return NO_PIECE;
    }
}