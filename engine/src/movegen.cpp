#include "movegen.hpp"
#include <math.h>

void generate_moves(const Position& pos, std::vector<uint16_t>& out) {
    out.clear();

    Color side_to_move = pos.get_side();
    Board board = pos.get_board();

    for (int sq = 0; sq < 64; sq++) {
        Piece piece = board.get(sq);
        if (piece == EMPTY) continue;
        if (!is_piece_of_side(piece, side_to_move)) continue;

        PieceType type = get_piece_type(piece);
    }
}

void generate_pawn_moves(const Position& pos, int sq, Piece piece, std::vector<uint16_t>& out) {
    int forward, start_rank, promotion_rank;
    Color pawn_side;
    const Board& board = pos.get_board();

    if (is_white(piece)) {
        forward = WHITE_FORWARD;
        start_rank = 1;
        promotion_rank = 7;
        pawn_side = WHITE;
    }
    else {
        forward = BLACK_FORWARD;
        start_rank = 6;
        promotion_rank = 0;
        pawn_side = BLACK;
    }

    int rank = sq / 8;
    int file = sq % 8;

    // Single and double forward push
    int target_single_sq = sq + forward;
    int target_double_sq = target_single_sq + forward;
    int target_rank = target_single_sq / 8;

    if (target_single_sq >= 0 && target_single_sq < 64) {
        if (board.get(target_single_sq) == EMPTY) {
            // Single push
            if (target_rank == promotion_rank) {
                out.push_back(make_move(sq, target_single_sq, MoveType::KNIGHT_PROMO));
                out.push_back(make_move(sq, target_single_sq, MoveType::BISHOP_PROMO));
                out.push_back(make_move(sq, target_single_sq, MoveType::ROOK_PROMO));
                out.push_back(make_move(sq, target_single_sq, MoveType::QUEEN_PROMO));
            }
            else {
                out.push_back(make_move(sq, target_single_sq, MoveType::QUIET));
            }
            
            // Double push
            if (rank == start_rank) {
                if (board.get(target_double_sq) == EMPTY) {
                    out.push_back(make_move(sq, target_double_sq, MoveType::DOUBLE_PUSH));
                }
            }
        }
    }

    // Captures left and right
    for (int file_offset : {-1, +1}) {
        int target_file = file + file_offset;
        if (target_file < 0 || target_file > 7) continue;

        int target_sq = sq + forward + file_offset;
        
        Piece target = board.get(target_sq);

        if (target == EMPTY) {
            // En passant
            int en_passant_sq = pos.get_ep_sq();
            if (en_passant_sq == target_sq) {
                out.push_back(make_move(sq, target_sq, MoveType::EN_PASSANT));
            }
            continue;
        }

        if (is_piece_of_side(target, pawn_side)) continue;

        int target_rank = target_sq / 8;
        if (target_rank == promotion_rank) {
            out.push_back(make_move(sq, target_sq, MoveType::KNIGHT_PROMO_CAPTURE));
            out.push_back(make_move(sq, target_sq, MoveType::BISHOP_PROMO_CAPTURE));
            out.push_back(make_move(sq, target_sq, MoveType::ROOK_PROMO_CAPTURE));
            out.push_back(make_move(sq, target_sq, MoveType::QUEEN_PROMO_CAPTURE));
        }
        else {
            out.push_back(make_move(sq, target_sq, MoveType::CAPTURE));
        }
    }
}

void generate_knight_moves(const Position& pos, int sq, Piece piece, std::vector<uint16_t>& out) {
    const Board& board = pos.get_board();
    int file = sq % 8;
    Color piece_color = is_white(piece) ? WHITE : BLACK;
    
    for (int offset: KNIGHT_OFFSETS) {
        int target_sq = sq + offset;
        if (target_sq < 0 || target_sq >= 64) continue;
        
        int target_file = target_sq % 8;
        Piece target = board.get(target_sq);
        
        if (abs(target_file-file) > 2) continue;

        if (target == EMPTY) {
            out.push_back(make_move(sq, target_sq, QUIET));
        }
        if (target != EMPTY && !is_piece_of_side(target, piece_color)) {
            out.push_back(make_move(sq, target_sq, MoveType::CAPTURE));
        }
    }
}
