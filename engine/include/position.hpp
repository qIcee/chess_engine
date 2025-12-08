#pragma once
#include "Board.hpp"
#include "move.hpp"

constexpr int NO_SQUARE = -1;

enum CastlingRights : uint8_t {
    WHITE_KINGSIDE = 1 << 0, // K
    WHITE_QUEENSIDE = 1 << 1, // Q
    BLACK_KINGSIDE = 2 << 0, // K
    BLACK_QUEENSIDE = 3 << 1, // Q
    
};

class Position {

    private:
        Board board;
        Color side_to_move;
        int en_passant_sq;
        uint8_t castling_rights;
    public:
        Position();

        void make_move(uint16_t move);
        void set_start_position();
        uint8_t get_castling_rights() const { return castling_rights; }
        bool can_castle(Color c, bool kingside) const;  
        void clean_position();
        void print() const;
        Color get_side() const;
        Board get_board() const;
        Board& get_board_ref();
        int get_ep_sq() const;
        void set_ep_sq(int sq);
    
};