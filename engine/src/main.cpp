#include <iostream>
#include <vector>

#include "position.hpp"  // your Position / Board / Color
#include "move.hpp"      // make_move, from_sq, to_sq, move_type
#include "piece.hpp"     // Piece, WP, BP, etc.
#include "movegen.hpp"

// --- Helper to pretty-print a move ---
void print_move(uint16_t m) {
    std::cout << "from=" << from_sq(m)
              << " to="   << to_sq(m)
              << " type=" << move_type(m)
              << "\n";
}

void print_moves(const std::vector<uint16_t>& moves, const char* label) {
    std::cout << "=== " << label << " ===\n";
    for (uint16_t m : moves) {
        print_move(m);
    }
    std::cout << std::endl;
}

// You may need to adapt how you get/set the board inside Position.
// Here I assume Position has:
//   - Board&       Position::board_ref();        // non-const ref to its board
//   - void         Position::set_side(Color);
//   - void         Position::set_ep_sq(int);
//   - Color        WHITE, BLACK;
// Adjust names as needed.

void test_white_single_and_double_push() {
    Position pos;
    pos.set_start_position();
    pos.set_ep_sq(-1);           // no en passant

    Board& board = pos.get_board_ref();
    // e2: rank 1, file 4 -> 1*8 + 4 = 12
    int e2 = 1 * 8 + 4;
    board.set(e2, Piece::WP);

    std::vector<uint16_t> moves;
    
    generate_pawn_moves(pos, e2, WP, moves);

    print_moves(moves, "White pawn e2: single + double push");
}

void test_white_blocked() {
    Position pos;
    pos.set_start_position();
    pos.set_ep_sq(-1);

    Board& board = pos.get_board_ref();

    int e2 = 1 * 8 + 4;  // e2
    int e3 = 2 * 8 + 4;  // e3

    board.set(e2, WP);
    board.set(e3, BP);   // something blocking

    std::vector<uint16_t> moves;
    generate_pawn_moves(pos, e2, WP, moves);

    print_moves(moves, "White pawn e2 blocked by piece on e3");
}

void test_white_promotion_quiet() {
    Position pos;
    pos.set_start_position();
    pos.set_ep_sq(-1);

    Board& board = pos.get_board_ref();

    int a7 = 6 * 8 + 0;  // rank 6, file 0
    board.set(a7, WP);   // white pawn on a7

    std::vector<uint16_t> moves;
    generate_pawn_moves(pos, a7, WP, moves);

    print_moves(moves, "White pawn a7 promotion (quiet)");
}

void test_white_promotion_capture() {
    Position pos;
    pos.set_start_position();
    pos.set_ep_sq(-1);

    Board& board = pos.get_board_ref();

    int g7 = 6 * 8 + 6;  // g7
    int h8 = 7 * 8 + 7;  // h8

    board.set(g7, WP);
    board.set(h8, BR);   // black rook to be captured

    std::vector<uint16_t> moves;
    generate_pawn_moves(pos, g7, WP, moves);

    print_moves(moves, "White pawn g7 promotion capture on h8");
}

void test_white_en_passant() {
    Position pos;
    pos.set_start_position();

    Board& board = pos.get_board_ref();

    // White pawn on e5
    int e5 = 4 * 8 + 4;  // rank 4, file 4
    board.set(e5, WP);

    // Black pawn that just played d7 -> d5 and now sits on d5
    int d5 = 4 * 8 + 3;  // rank 4, file 3
    board.set(d5, BP);

    // En passant square is the jumped-over square d6
    int d6 = 5 * 8 + 3;  // rank 5, file 3
    pos.set_ep_sq(d6);

    std::vector<uint16_t> moves;
    generate_pawn_moves(pos, e5, WP, moves);

    print_moves(moves, "White pawn e5 en passant on d6");
}

uint64_t perft(Position& pos, int depth) {
    if (depth == 0) return 1;

    std::vector<uint16_t> moves;
    generate_legal_moves(pos, moves); 

    uint64_t nodes = 0;

    for (uint16_t m : moves) {
        pos.do_move(m);
        nodes += perft(pos, depth - 1);
        pos.undo_move();
    }

    return nodes;
}

int evaluate(const Position& pos) {
    int white = 0, black = 0;
    Board board = pos.get_board();
    for (int sq = 0; sq < 64; sq++) {
        Piece p = board.get(sq);
        if (p == EMPTY) continue;

        int v = piece_value(p);
        if (is_white(p)) white += v;
        else black += v;
    }

    int score_white_pov = white - black;

    return (pos.get_side() == WHITE) ? score_white_pov : -score_white_pov;
}

int negamax(Position& pos, int depth, int alpha, int beta) {
    if (depth == 0)
        return evaluate(pos);

    std::vector<uint16_t> moves;
    generate_legal_moves(pos, moves);

    if (moves.empty()) {
        if (pos.in_check(pos.get_side()))
            return -MATE;
        else
            return 0;
    }

    int best = -INF;

    for (auto m : moves) {
        pos.do_move(m);
        int score = -negamax(pos, depth - 1, -beta, -alpha);
        pos.undo_move();

        if (score > best) best = score;
        if (score > alpha) alpha = score;
        if (alpha >= beta) break; // prune
    }

    return best;
}

uint16_t find_best_move(Position& pos, int depth) {
    std::vector<uint16_t> moves;
    generate_legal_moves(pos, moves);

    uint16_t bestMove = MOVE_NONE;
    int bestScore = -INF;

    for (auto m : moves) {
        pos.do_move(m);
        int score = -negamax(pos, depth - 1, -INF, INF);
        pos.undo_move();

        if (score > bestScore) {
            bestScore = score;
            bestMove = m;
        }
    }
    return bestMove;
}

static std::string sq_to_str(int sq) {
    char file = 'a' + (sq % 8);
    char rank = '1' + (sq / 8);
    return {file, rank};
}

static std::string move_to_str(uint16_t m) {
    if (m == MOVE_NONE) return "MOVE_NONE";
    return sq_to_str(from_sq(m)) + sq_to_str(to_sq(m));
}

int main() {
    Position pos;
    pos.set_start_position();
    // If you have FEN support, prefer this:
    // pos.set_fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    pos.get_board_ref().set(16, Piece(13));
    pos.get_board().print();

    int depth = 4;

    std::cout << "Searching depth " << depth << "...\n";

    uint16_t best = find_best_move(pos, depth);

    if (best == MOVE_NONE) {
        std::cout << "No legal moves (mate or stalemate)\n";
        return 0;
    }

    std::cout << "Best move: " << move_to_str(best) << "\n";

    // Optional: apply the move and print side to move
    pos.do_move(best);
    std::cout << "Side to move after move: "
              << (pos.get_side() == WHITE ? "White" : "Black")
              << "\n";

    return 0;
}
