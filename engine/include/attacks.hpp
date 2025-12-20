#pragma once
#include "piece.hpp"
#include "types.hpp"
#include <math.h>

class Position;


bool is_square_attacked(const Position& pos, int sq, Color side); 
int find_king_square(const Position& pos, Color side);

