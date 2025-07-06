#pragma once
#include "piece.hpp"

struct Board {
    Piece squares[64]; // 8x8 chessboard represented as a 1D array

    Piece get(int rank, int file) const;
    void set(int rank, int file, Piece piece);
    void reset();
    void print();
};