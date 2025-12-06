#include "position.hpp"
#include <iostream>

Position::Position() : side_to_move(WHITE) {}

void Position::make_move(uint16_t move) {
    int from = from_sq(move);
    int to = to_sq(move);
    int type = move_type(move);

    int ep_square = NO_SQUARE; // Default: no en passant after this move

    Piece p = board.get(from);

    // Move the piece normally
    board.set(to, p);
    board.set(from, EMPTY);

    // If it's a double pawn push, set ep_square
    if (type == DOUBLE_PUSH) {
        if (is_white(p))
            ep_square = to - 8;
        else 
            ep_square = to + 8;
    }

    // If it's an en passant capture, remove the pawn behind the landing square
    if (type == EN_PASSANT) {
        if (is_white(p)) 
            board.set(to - 8, EMPTY);
        else 
            board.set(to + 8, EMPTY);    
    }
}

void Position::set_start_position() {
    board.reset();
    side_to_move = WHITE;
}

void Position::clean_position() {
    board.clean_reset();
}

void Position::print() const {
    board.print();
    std::cout << (side_to_move == WHITE ? "White" : "Black") << " to move.\n";
}

Color Position::get_side() const {
    return side_to_move;
}

Board Position::get_board() const {
    return board;
}

Board& Position::get_board_ref() {
    return board;
}

int Position::get_ep_sq() const {
    return en_passant_sq;
}

void Position::set_ep_sq(int sq) {
    en_passant_sq = sq;
}