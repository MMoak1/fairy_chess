#include "Knight.h"
#include "Board.h"
#include <cmath>

using namespace std;

bool Knight::isValidMove(Board& board, int toRow, int toCol) {
    if (!board.isInBounds(toRow, toCol)) return false;
    int dr = abs(toRow - row), dc = abs(toCol - col);
    if (!((dr == 2 && dc == 1) || (dr == 1 && dc == 2))) return false;

    Piece* target = board.getPieceAt(toRow, toCol);
    if (target && target->getColor() == color) return false;
    return true;
}

vector<pair<int,int>> Knight::getPseudoLegalMoves(Board& board) {
    vector<pair<int,int>> moves;
    int offsets[8][2] = {{2,1},{2,-1},{-2,1},{-2,-1},{1,2},{1,-2},{-1,2},{-1,-2}};
    for (auto& o : offsets) {
        int r = row + o[0], c = col + o[1];
        if (board.isInBounds(r, c)) {
            Piece* target = board.getPieceAt(r, c);
            if (!target || target->getColor() != color)
                moves.push_back({r, c});
        }
    }
    return moves;
}