#pragma once
#include "Piece.h"

class Board; // forward declaration

class Pawn : public Piece {
public:
    Pawn(string color, int row, int col)
        : Piece(color, true, row, col, 'P') {}

    bool isValidMove(Board& board, int toRow, int toCol) override;
    vector<pair<int,int>> getPseudoLegalMoves(Board& board) override;
};
