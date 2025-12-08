#include "position.hpp"
#include <iostream>

Position::Position() : side_to_move(WHITE) {}

void Position::make_move(uint16_t move) {
    int from = from_sq(move);
    int to = to_sq(move);
    int type = move_type(move);

    int ep_square = NO_SQUARE; // Default: no en passant after this move

    Piece p = board.get(from);
    Color c = side_to_move;
    Piece target = board.get(to);
    // Move the piece normally
    board.set(to, p);
    board.set(from, EMPTY);


/*
    PAWN MOVES
*/
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


/*
    KING MOVES
*/
    // If a king moves, that side loses all castling rights
    if (p = WK)
        castling_rights &= ~(WHITE_KINGSIDE | WHITE_QUEENSIDE);
    else 
        castling_rights &= ~(BLACK_KINGSIDE | BLACK_QUEENSIDE);

    // IF a rook moves from its original space, lose only that side
    if (p = WR) {
        if (from == 7 /* h1 */) castling_rights &= ~WHITE_KINGSIDE;
        if (from == 0 /* a1 */) castling_rights &= ~WHITE_QUEENSIDE;
    }
    else if (p = BR) {
        if (from == 63 /* h8 */) castling_rights &= ~BLACK_KINGSIDE;
        if (from == 56 /* a8 */) castling_rights &= ~BLACK_QUEENSIDE;
    }

    // If a rook on those original squares gets captured, lose those rights too
    if (target == WR) {
        if (to == 7)  castling_rights &= ~WHITE_KINGSIDE;
        if (to == 0)  castling_rights &= ~WHITE_QUEENSIDE;
    } else if (target == BR) {
        if (to == 63) castling_rights &= ~BLACK_KINGSIDE;
        if (to == 56) castling_rights &= ~BLACK_QUEENSIDE;
    }
}

void Position::set_start_position() {
    board.reset();
    side_to_move = WHITE;
    castling_rights = WHITE_KINGSIDE | WHITE_QUEENSIDE | BLACK_KINGSIDE | BLACK_QUEENSIDE;
}

bool Position::can_castle(Color c, bool king_side) const {
    if (c == WHITE) 
        return king_side ? (castling_rights & WHITE_KINGSIDE) : (castling_rights & WHITE_QUEENSIDE);
    else 
        return king_side ? (castling_rights & BLACK_KINGSIDE) : (castling_rights & BLACK_QUEENSIDE);
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