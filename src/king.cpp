#include "King.h"
#include "Board.h"
#include <cmath>

using namespace std;

bool King::isValidMove(Board& board, int toRow, int toCol) {
    if (!board.isInBounds(toRow, toCol)) return false;
    if (toRow == row && toCol == col) return false;

    int dr = abs(toRow - row), dc = abs(toCol - col);
    // Normal king move: 1 square in any direction
    if (dr > 1 || dc > 1) return false;

    Piece* target = board.getPieceAt(toRow, toCol);
    if (target && target->getColor() == color) return false;
    return true;
}

vector<pair<int,int>> King::getPseudoLegalMoves(Board& board) {
    vector<pair<int,int>> moves;
    for (int dr = -1; dr <= 1; dr++) {
        for (int dc = -1; dc <= 1; dc++) {
            if (dr == 0 && dc == 0) continue;
            int r = row + dr, c = col + dc;
            if (board.isInBounds(r, c)) {
                Piece* target = board.getPieceAt(r, c);
                if (!target || target->getColor() != color)
                    moves.push_back({r, c});
            }
        }
    }
    return moves;
}