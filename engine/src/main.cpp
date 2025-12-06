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

int main() {
    test_white_single_and_double_push();
    test_white_blocked();
    test_white_promotion_quiet();
    test_white_promotion_capture();
    test_white_en_passant();

    return 0;
}
