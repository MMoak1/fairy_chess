#pragma once
#include "Piece.h"

class Board;

class Rook : public Piece {
public:
    Rook(string color, int row, int col)
        : Piece(color, true, row, col, 'R') {}

    bool isValidMove(Board& board, int toRow, int toCol) override;
    vector<pair<int,int>> getPseudoLegalMoves(Board& board) override;
};
