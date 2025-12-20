#pragma once
#include "Board.hpp"
#include "move.hpp"
#include <vector>

constexpr int NO_SQUARE = -1;
constexpr int MATE = 30000;
constexpr int INF = 32000;
constexpr uint16_t MOVE_NONE = 0;

enum CastlingRights : uint8_t {
    WHITE_KINGSIDE = 1 << 0, // K
    WHITE_QUEENSIDE = 1 << 1, // Q
    BLACK_KINGSIDE = 2 << 0, // K
    BLACK_QUEENSIDE = 3 << 1, // Q
    
};

struct HistoryEntry {
    uint16_t move;
    Piece captured;
    int prev_ep_sq;
    uint8_t prev_castling;
};

class Position {
    private:
        Board board;
        Color side_to_move;
        int en_passant_sq;
        uint8_t castling_rights;
        std::vector<HistoryEntry> history;
    public:
        Position();

        void do_move(uint16_t move);
        void undo_move();
        void set_start_position();
        uint8_t get_castling_rights() const { return castling_rights; }
        bool can_castle(Color c, bool kingside) const;  
        bool in_check(Color side);
        void clean_position();
        void print() const;
        Color get_side() const;
        Board get_board() const;
        Board& get_board_ref();
        int get_ep_sq() const;
        void set_ep_sq(int sq);
        void do_pawn_move(uint16_t move, Piece pawn, int from, int to, MoveType type);
        void do_king_move(Piece king, int from, int to, MoveType type);
        std::vector<HistoryEntry> get_history() const { return history; }
        void push_history(HistoryEntry entry);
        HistoryEntry pop_history();
    
};