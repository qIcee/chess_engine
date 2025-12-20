#include "attacks.hpp"
#include "position.hpp"
bool is_square_attacked(const Position& pos, int sq, Color side) {
    const Board& board = pos.get_board();
    const int rank = sq / 8;
    const int file = sq % 8;

    // --- Pawn attacks ---
    // We look "backwards" from sq: from which squares could a pawn of `side`
    // be sitting that attacks sq?
    if (side == WHITE) {
        // White pawns move north, so they attack from south-east / south-west
        Piece p = board.get(sq + SOUTH_EAST);
        if (p != EMPTY && is_piece_of_side(p, side) && get_piece_type(p) == PAWN)
            return true;

        p = board.get(sq + SOUTH_WEST);
        if (p != EMPTY && is_piece_of_side(p, side) && get_piece_type(p) == PAWN)
            return true;

    } else { // side == BLACK
        // Black pawns move south, so they attack from north-east / north-west
        Piece p = board.get(sq + NORTH_EAST);
        if (p != EMPTY && is_piece_of_side(p, side) && get_piece_type(p) == PAWN)
            return true;

        p = board.get(sq + NORTH_WEST);
        if (p != EMPTY && is_piece_of_side(p, side) && get_piece_type(p) == PAWN)
            return true;
    }

    // --- Knight attacks ---
    for (int offset : KNIGHT_OFFSETS) {
        int target_sq = sq + offset;
        if (target_sq < 0 || target_sq >= 64) continue;

        // Avoid wrap-around: knight’s file change must be <= 2
        int target_file = target_sq % 8;
        if (std::abs(target_file - file) > 2) continue;

        Piece p = board.get(target_sq);
        if (p == EMPTY || !is_piece_of_side(p, side)) continue;

        if (get_piece_type(p) == KNIGHT)
            return true;
    }

    // --- Bishop & Queen (diagonals) ---
    for (int direction : BISHOP_DIRS) {
        int current_sq = sq;

        while (true) {
            current_sq += direction;
            if (current_sq < 0 || current_sq >= 64) break;

            int current_rank = current_sq / 8;
            int current_file = current_sq % 8;

            // Must stay on same diagonal; otherwise we've wrapped
            if (std::abs(current_rank - rank) != std::abs(current_file - file))
                break;

            Piece p = board.get(current_sq);
            if (p == EMPTY) continue;

            if (!is_piece_of_side(p, side)) {
                // Blocked by other color
                break;
            }

            PieceType pt = get_piece_type(p);
            if (pt == BISHOP || pt == QUEEN)
                return true;

            // Own piece of wrong type blocks the ray
            break;
        }
    }

    // --- Rook & Queen (ranks/files) ---
    for (int direction : ROOK_DIRS) {
        int current_sq = sq;

        while (true) {
            current_sq += direction;
            if (current_sq < 0 || current_sq >= 64) break;

            int current_rank = current_sq / 8;
            int current_file = current_sq % 8;

            // Prevent horizontal wrap: if direction is EAST/WEST, rank must stay same
            // Prevent vertical wrap: if direction is NORTH/SOUTH, file must stay same
            bool horizontal = (direction == EAST || direction == WEST);
            if (horizontal && current_rank != rank)
                break;

            bool vertical = (direction == NORTH || direction == SOUTH);
            if (vertical && current_file != file)
                break;

            Piece p = board.get(current_sq);
            if (p == EMPTY) continue;

            if (!is_piece_of_side(p, side)) {
                // Blocked by other color
                break;
            }

            PieceType pt = get_piece_type(p);
            if (pt == ROOK || pt == QUEEN)
                return true;

            // Own piece of wrong type blocks the ray
            break;
        }
    }

    // --- King attacks ---
    for (int offset : KING_OFFSETS) {
        int target_sq = sq + offset;
        if (target_sq < 0 || target_sq >= 64) continue;

        int target_file = target_sq % 8;
        if (std::abs(target_file - file) > 1) continue; // avoid wrap

        Piece p = board.get(target_sq);
        if (p == EMPTY || !is_piece_of_side(p, side)) continue;

        if (get_piece_type(p) == KING)
            return true;
    }

    return false;
}

int find_king_square(const Position& pos, Color side) {
    const Board& board = pos.get_board();
    Piece king = (side == WHITE ? WK : BK);

    for (int sq = 0; sq < 64; ++sq) {
        if (board.get(sq) == king) {
            return sq;
        }
    }
    return -1; // should never happen if position is valid
}