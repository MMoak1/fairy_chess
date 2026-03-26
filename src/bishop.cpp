#include "Bishop.h"
#include "Board.h"
#include <cmath>

using namespace std;

bool Bishop::isValidMove(Board& board, int toRow, int toCol) {
    if (!board.isInBounds(toRow, toCol)) return false;
    if (toRow == row && toCol == col) return false;
    // Must move diagonally
    if (abs(toRow - row) != abs(toCol - col)) return false;

    int dr = (toRow > row) ? 1 : -1;
    int dc = (toCol > col) ? 1 : -1;
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

vector<pair<int,int>> Bishop::getPseudoLegalMoves(Board& board) {
    vector<pair<int,int>> moves;
    int dirs[4][2] = {{1,1},{1,-1},{-1,1},{-1,-1}};
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