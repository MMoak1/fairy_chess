#pragma once
#include "Piece.h"

class Board;

class Queen : public Piece {
public:
    Queen(string color, int row, int col)
        : Piece(color, true, row, col, 'Q') {}

    bool isValidMove(Board& board, int toRow, int toCol) override;
    vector<pair<int,int>> getPseudoLegalMoves(Board& board) override;
};
