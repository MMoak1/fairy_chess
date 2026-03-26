#pragma once
#include "Piece.h"

class Board;

class King : public Piece {
public:
    King(string color, int row, int col)
        : Piece(color, true, row, col, 'K') {}

    bool isValidMove(Board& board, int toRow, int toCol) override;
    vector<pair<int,int>> getPseudoLegalMoves(Board& board) override;
};
