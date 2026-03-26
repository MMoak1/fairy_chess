#include "Rook.h"
#include "Board.h"

using namespace std;

bool Rook::isValidMove(Board& board, int toRow, int toCol) {
    if (!board.isInBounds(toRow, toCol)) return false;
    if (toRow == row && toCol == col) return false;
    // Must move in a straight line (same row or same col)
    if (toRow != row && toCol != col) return false;

    // Check path is clear
    int dr = (toRow > row) ? 1 : (toRow < row) ? -1 : 0;
    int dc = (toCol > col) ? 1 : (toCol < col) ? -1 : 0;
    int r = row + dr, c = col + dc;
    while (r != toRow || c != toCol) {
        if (board.getPieceAt(r, c)) return false;
        r += dr;
        c += dc;
    }

    // Destination must be empty or have enemy piece
    Piece* target = board.getPieceAt(toRow, toCol);
    if (target && target->getColor() == color) return false;
    return true;
}

vector<pair<int,int>> Rook::getPseudoLegalMoves(Board& board) {
    vector<pair<int,int>> moves;
    int dirs[4][2] = {{1,0},{-1,0},{0,1},{0,-1}};
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