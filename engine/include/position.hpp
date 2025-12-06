#pragma once
#include "Board.hpp"
#include "move.hpp"

constexpr int NO_SQUARE = -1;

class Position {

    private:
        Board board;
        Color side_to_move;
        int en_passant_sq;
    public:
        Position();

        void make_move(uint16_t move);
        void set_start_position();
        void clean_position();
        void print() const;
        Color get_side() const;
        Board get_board() const;
        Board& get_board_ref();
        int get_ep_sq() const;
        void set_ep_sq(int sq);
    
};