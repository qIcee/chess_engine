#include "position.hpp"
#include "types.hpp"
#include "move.hpp"

#include <vector>
#include <cstdint>



// Generates all moves
void generate_legal_moves(Position& pos, std::vector<uint16_t>& out); 
void generate_moves(const Position& pos, std::vector<uint16_t>& out); 
void generate_pawn_moves(const Position& pos, int sq, Piece piece, std::vector<uint16_t>& out);
void generate_knight_moves(const Position& pos, int sq, Piece piece, std::vector<uint16_t>& out);
void generate_king_moves(const Position& pos, int sq, Piece piece, std::vector<uint16_t>& out);
void generate_rook_moves(const Position& pos, int sq, Piece piece, std::vector<uint16_t>& out);
void generate_bishop_moves(const Position& pos, int sq, Piece piece, std::vector<uint16_t>& out);
void generate_queen_moves(const Position& pos, int sq, Piece piece, std::vector<uint16_t>& out);
