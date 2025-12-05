#include "position.hpp"
#include "types.hpp"
#include "move.hpp"
#include <vector>
#include <cstdint>



// Generates all pseudolegal moves
void generate_moves(const Position& pos, std::vector<uint16_t>& out); 
