#include "board.hpp"
#include "piece.hpp"

#include <iostream>

Piece Board::get(int rank, int file) const {
    if (rank < 0 || rank >= 8 || file < 0 || file >= 8) {
        return EMPTY; // Return EMPTY if out of bounds
    }
    return squares[rank * 8 + file];
}

Piece Board::get(int idx) const {
    return squares[idx];
}

void Board::set(int rank, int file, Piece piece) {
    if (rank < 0 || rank >= 8 || file < 0 || file >= 8) {
        return; // Do nothing if out of bounds
    }
    squares[rank * 8 + file] = piece;
}

void Board::reset() {
    // reset board
    for (int i = 0; i < 64; i++) {
        squares[i] = EMPTY;
    }

    // Fill in white pieces
    Piece white_back[] = { WR, WN, WB, WQ, WK, WB, WN, WR };
    for (int file = 0; file < 8; ++file) {
        set(0, file, white_back[file]);
        set(1, file, WP);
    }

    // Fill in black pieces
    Piece black_back[] = { BR, BN, BB, BQ, BK, BB, BN, BR };
    for (int file = 0; file < 8; ++file) {
        set(7, file, black_back[file]);
        set(6, file, BP);
    }
}

void Board::print() const {
    for (int rank = 7; rank >= 0; --rank) {
        std::cout << (rank + 1) << " ";
        for (int file = 0; file < 8; file++) {
            Piece p = get(rank, file);
            char c = '.';

            switch(p) {
                case WP: c = 'P'; break;
                case WN: c = 'N'; break;
                case WB: c = 'B'; break;
                case WR: c = 'R'; break;
                case WQ: c = 'Q'; break;
                case WK: c = 'K'; break;
                case BP: c = 'p'; break;
                case BN: c = 'n'; break;
                case BB: c = 'b'; break;
                case BR: c = 'r'; break;
                case BQ: c = 'q'; break;
                case BK: c = 'k'; break;
                case EMPTY: c = '.'; break;
            }
            
            std::cout << c << " ";
        }
        std::cout << std::endl;
    }

    std::cout << "  a b c d e f g h" << std::endl;
}