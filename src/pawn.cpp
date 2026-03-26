#include "Pawn.h"
#include "Board.h"
#include <cmath>

using namespace std;

bool Pawn::isValidMove(Board& board, int toRow, int toCol) {
    if (!board.isInBounds(toRow, toCol)) return false;
    Piece* target = board.getPieceAt(toRow, toCol);

    int direction = (color == "white") ? 1 : -1;
    int startRow = (color == "white") ? 1 : 6;

    // Forward 1 square
    if (toCol == col && toRow == row + direction && !target) {
        return true;
    }

    // Forward 2 squares from starting position
    if (toCol == col && toRow == row + 2 * direction && row == startRow && !target) {
        // Must not be blocked
        if (!board.getPieceAt(row + direction, col))
            return true;
    }

    // Diagonal capture
    if (abs(toCol - col) == 1 && toRow == row + direction) {
        // Normal capture
        if (target && target->getColor() != color)
            return true;
        // En passant
        if (toRow == board.getEnPassantRow() && toCol == board.getEnPassantCol())
            return true;
    }

    return false;
}

vector<pair<int,int>> Pawn::getPseudoLegalMoves(Board& board) {
    vector<pair<int,int>> moves;
    int direction = (color == "white") ? 1 : -1;
    int startRow = (color == "white") ? 1 : 6;

    // Forward 1
    int nr = row + direction;
    if (board.isInBounds(nr, col) && !board.getPieceAt(nr, col)) {
        moves.push_back({nr, col});
        // Forward 2
        int nr2 = row + 2 * direction;
        if (row == startRow && board.isInBounds(nr2, col) && !board.getPieceAt(nr2, col)) {
            moves.push_back({nr2, col});
        }
    }
    // Diagonal captures
    for (int dc : {-1, 1}) {
        int nc = col + dc;
        if (board.isInBounds(nr, nc)) {
            Piece* target = board.getPieceAt(nr, nc);
            if (target && target->getColor() != color)
                moves.push_back({nr, nc});
            if (nr == board.getEnPassantRow() && nc == board.getEnPassantCol())
                moves.push_back({nr, nc});
        }
    }
    return moves;
}