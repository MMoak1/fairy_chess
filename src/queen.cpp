#include "Queen.h"
#include "Board.h"
#include <cmath>

using namespace std;

bool Queen::isValidMove(Board& board, int toRow, int toCol) {
    if (!board.isInBounds(toRow, toCol)) return false;
    if (toRow == row && toCol == col) return false;

    bool straight = (toRow == row || toCol == col);
    bool diagonal = (abs(toRow - row) == abs(toCol - col));
    if (!straight && !diagonal) return false;

    int dr = (toRow > row) ? 1 : (toRow < row) ? -1 : 0;
    int dc = (toCol > col) ? 1 : (toCol < col) ? -1 : 0;
    int r = row + dr, c = col + dc;
    while (r != toRow || c != toCol) {
        if (board.getPieceAt(r, c)) return false;
        r += dr;
        c += dc;
    }

    Piece* target = board.getPieceAt(toRow, toCol);
    if (target && target->getColor() == color) return false;
    return true;
}

vector<pair<int,int>> Queen::getPseudoLegalMoves(Board& board) {
    vector<pair<int,int>> moves;
    int dirs[8][2] = {{1,0},{-1,0},{0,1},{0,-1},{1,1},{1,-1},{-1,1},{-1,-1}};
    for (auto& d : dirs) {
        int r = row + d[0], c = col + d[1];
        while (board.isInBounds(r, c)) {
            Piece* target = board.getPieceAt(r, c);
            if (!target) {
                moves.push_back({r, c});
            } else {
                if (target->getColor() != color)
                    moves.push_back({r, c});
                break;
            }
            r += d[0];
            c += d[1];
        }
    }
    return moves;
}