#include "position.hpp"
#include <iostream>

Position::Position() : side_to_move(WHITE) {}

void Position::do_move(uint16_t move) {
    int from      = from_sq(move);
    int to        = to_sq(move);
    MoveType type = static_cast<MoveType>(move_type(move));
    
    // Add to history
    HistoryEntry entry;
    entry.move = move;
    entry.captured = board.get(to);
    entry.prev_ep_sq = get_ep_sq();
    entry.prev_castling = get_castling_rights();
    push_history(entry);

    // Clear en passant by default for this move
    set_ep_sq(NO_SQUARE);

    Piece p       = board.get(from);
    Piece captured = board.get(to); // piece that was on 'to' before move (may be EMPTY)
    PieceType pt  = get_piece_type(p);

    Color c = side_to_move;

    // --- Piece-type specific handling ---
    if (pt == PAWN) {
        // You implement this as before
        do_pawn_move(move, p, from, to, type);

    } else if (pt == KING) {
        do_king_move(p, from, to, type);

    } else {
        // Generic non-pawn, non-king move (knight, bishop, rook, queen)
        board.set(from, EMPTY);
        board.set(to, p);

        // Rook moves from original squares -> lose corresponding castling rights
        if (p == WR) {
            if (from == 7)  castling_rights &= ~WHITE_KINGSIDE;   // h1
            if (from == 0)  castling_rights &= ~WHITE_QUEENSIDE;  // a1
        } else if (p == BR) {
            if (from == 63) castling_rights &= ~BLACK_KINGSIDE;   // h8
            if (from == 56) castling_rights &= ~BLACK_QUEENSIDE;  // a8
        }
    }

    // --- Captured rook on original square -> adjust castling rights ---
    if (captured == WR) {
        if (to == 7)  castling_rights &= ~WHITE_KINGSIDE;
        if (to == 0)  castling_rights &= ~WHITE_QUEENSIDE;
    } else if (captured == BR) {
        if (to == 63) castling_rights &= ~BLACK_KINGSIDE;
        if (to == 56) castling_rights &= ~BLACK_QUEENSIDE;
    }

    // --- Switch side to move ---
    side_to_move = (side_to_move == WHITE ? BLACK : WHITE);
}

void Position::undo_move() {
    // 1. Get last history entry
    HistoryEntry entry = pop_history();

    uint16_t move = entry.move;
    int from      = from_sq(move);
    int to        = to_sq(move);
    MoveType type = static_cast<MoveType>(move_type(move));

    // 2. Side that made the move (current side_to_move is the OPPONENT)
    Color mover = (side_to_move == WHITE ? BLACK : WHITE);

    // 3. Restore ep square and castling rights
    set_ep_sq(entry.prev_ep_sq);
    castling_rights = entry.prev_castling;

    // 4. Piece currently on 'to' is the piece that moved (or promoted piece)
    Piece moved    = board.get(to);
    PieceType pt   = get_piece_type(moved);

    // Pawn move detection:
    // - normal pawn moves: pt == PAWN
    // - promotions: pt != PAWN, but type is in promo range
    bool is_promo_type =
        (type == KNIGHT_PROMO || type == BISHOP_PROMO ||
         type == ROOK_PROMO   || type == QUEEN_PROMO  ||
         type == KNIGHT_PROMO_CAPTURE || type == BISHOP_PROMO_CAPTURE ||
         type == ROOK_PROMO_CAPTURE   || type == QUEEN_PROMO_CAPTURE);

    bool is_pawn_move = (pt == PAWN) || is_promo_type;

    // 5. Undo board changes based on piece / move type
    if (is_pawn_move) {
        Piece pawn      = (mover == WHITE ? WP : BP);
        Piece opp_pawn  = (mover == WHITE ? BP : WP);

        if (type == EN_PASSANT) {
            // Pawn moved diagonally onto an empty 'to' square, capturing pawn behind
            board.set(from, pawn);
            board.set(to, EMPTY);

            int cap_sq = (mover == WHITE ? (to - 8) : (to + 8));
            board.set(cap_sq, opp_pawn);

        } else if (is_promo_type) {
            // Promotion (with or without capture)
            // At do_move: pawn from 'from' -> promoted piece on 'to', overwriting 'captured'
            // Undo: restore pawn on 'from', restore captured piece on 'to'
            board.set(from, pawn);
            board.set(to, entry.captured);

        } else {
            // Normal pawn move (quiet, capture, double push)
            // At do_move: from->EMPTY, to->pawn, captured stored in entry.captured
            board.set(from, moved);           // moved is the pawn
            board.set(to, entry.captured);    // restore whatever was on 'to' before
        }

    } else if (pt == KING) {
        // King moves (normal or castling)
        // At do_move: from->EMPTY, to->king, rook moved in castling
        // Undo: move king back and, if castling, move rook back

        // First undo rook movement if castling
        if (type == KING_CASTLE) {
            if (mover == WHITE) {
                // White O-O: rook f1 (5) -> h1 (7)
                board.set(7, WR);
                board.set(5, EMPTY);
            } else {
                // Black O-O: rook f8 (61) -> h8 (63)
                board.set(63, BR);
                board.set(61, EMPTY);
            }
        } else if (type == QUEEN_CASTLE) {
            if (mover == WHITE) {
                // White O-O-O: rook d1 (3) -> a1 (0)
                board.set(0, WR);
                board.set(3, EMPTY);
            } else {
                // Black O-O-O: rook d8 (59) -> a8 (56)
                board.set(56, BR);
                board.set(59, EMPTY);
            }
        }

        // Now move king back
        board.set(from, moved);
        board.set(to, entry.captured);  // usually EMPTY for castling, but safe

        // If you track king squares:
        // king_sq[mover] = from;

    } else {
        // Generic piece move (knight, bishop, rook, queen)
        // At do_move: from->EMPTY, to->moved, captured stored in entry.captured
        board.set(from, moved);
        board.set(to, entry.captured);

        // Castling rights are already restored from entry.prev_castling;
        // no extra rook logic needed here.
    }

    // 6. Restore side to move
    side_to_move = mover;
}

void Position::do_pawn_move(uint16_t move, Piece pawn, int from, int to, MoveType type) {
    bool white = is_white(pawn);

    // 1) Clear from square
    board.set(from, EMPTY);

    // 2) Decide which piece ends up on `to` (promotion or pawn)
    Piece result = pawn;

    switch (type) {
        case KNIGHT_PROMO:
        case KNIGHT_PROMO_CAPTURE:
            result = white ? WN : BN;
            break;

        case BISHOP_PROMO:
        case BISHOP_PROMO_CAPTURE:
            result = white ? WB : BB;
            break;

        case ROOK_PROMO:
        case ROOK_PROMO_CAPTURE:
            result = white ? WR : BR;
            break;

        case QUEEN_PROMO:
        case QUEEN_PROMO_CAPTURE:
            result = white ? WQ : BQ;
            break;

        default:
            // non-promotion pawn move: keep as pawn
            break;
    }

    // 3) Place the resulting piece on `to`
    board.set(to, result);

    // 4) Double pawn push → set en passant square
    if (type == DOUBLE_PUSH) {
        int ep_target = white ? (to - 8) : (to + 8);
        set_ep_sq(ep_target);
    }

    // 5) En passant capture → remove captured pawn behind `to`
    if (type == EN_PASSANT) {
        int cap_sq = white ? (to - 8) : (to + 8);
        board.set(cap_sq, EMPTY);
    }
}

void Position::do_king_move(Piece king, int from, int to, MoveType type) {
    Color side = is_white(king) ? WHITE : BLACK;

    // 1) Move king on board
    board.set(from, EMPTY);
    board.set(to, king);

    // 2) (Optional) track king square
    // if you add: int king_sq[2]; in Position
    // king_sq[side] = to;

    // 3) Clear castling rights for this side
    if (side == WHITE) {
        castling_rights &= ~(WHITE_KINGSIDE | WHITE_QUEENSIDE);
    } else {
        castling_rights &= ~(BLACK_KINGSIDE | BLACK_QUEENSIDE);
    }

    // 4) Handle rook movement in castling
    if (type == KING_CASTLE) {
        if (side == WHITE) {
            // White: e1 (4) -> g1 (6), rook h1 (7) -> f1 (5)
            board.set(7, EMPTY);
            board.set(5, WR);
        } else {
            // Black: e8 (60) -> g8 (62), rook h8 (63) -> f8 (61)
            board.set(63, EMPTY);
            board.set(61, BR);
        }
    } else if (type == QUEEN_CASTLE) {
        if (side == WHITE) {
            // White: e1 (4) -> c1 (2), rook a1 (0) -> d1 (3)
            board.set(0, EMPTY);
            board.set(3, WR);
        } else {
            // Black: e8 (60) -> c8 (58), rook a8 (56) -> d8 (59)
            board.set(56, EMPTY);
            board.set(59, BR);
        }
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

void Position::push_history(HistoryEntry entry) {
    history.push_back(entry);
}

HistoryEntry Position::pop_history() {
    HistoryEntry top = history.back();
    history.pop_back();
    return top;
}