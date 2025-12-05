#include "position.hpp"
#include <iostream>

Position::Position() : side_to_move(WHITE) {}

void Position::set_start_position() {
    board.reset();
    side_to_move = WHITE;
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