#pragma once
#include "Piece.h"

class Board;

class Knight : public Piece {
public:
    Knight(string color, int row, int col)
        : Piece(color, true, row, col, 'N') {}

    bool isValidMove(Board& board, int toRow, int toCol) override;
    vector<pair<int,int>> getPseudoLegalMoves(Board& board) override;
};
