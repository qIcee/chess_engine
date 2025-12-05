#pragma once
#include "Board.hpp"

enum Color {
    WHITE = 0,
    BLACK = 1,
    NO_COLOR = 2
};

class Position {

    private:
        Board board;
        Color side_to_move;

    public:
        Position();

        void set_start_position();
        void print() const;
        Color get_side() const;
        Board get_board() const;
    
};