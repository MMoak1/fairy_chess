#pragma once
#include "Piece.h"

class Board;

class Bishop : public Piece {
public:
    Bishop(string color, int row, int col)
        : Piece(color, true, row, col, 'B') {}

    bool isValidMove(Board& board, int toRow, int toCol) override;
    vector<pair<int,int>> getPseudoLegalMoves(Board& board) override;
};
