#pragma once
#include "piece.hpp"

// Squares are indexed 0..63 as rank * 8 + file, with 0 = a1, ..., 7 = h1, 8 = a2, ..., 63 = h8

struct Board {
    Piece squares[64]; // 8x8 chessboard represented as a 1D array

    Piece get(int rank, int file) const;
    Piece get(int idx) const;
    void set(int rank, int file, Piece piece);
    void set(int idx, Piece piece);
    void reset();
    void clean_reset();
    void print() const;
};