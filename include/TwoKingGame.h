#pragma once
#include "Game.h"
#include "Pawn.h"
#include "Rook.h"
#include "Knight.h"
#include "Bishop.h"
#include "King.h"
#include <cmath>
#include <iostream>

using namespace std;

// Two-King Chess: Queen is replaced by a second King.
// Win by capturing EITHER enemy king.
// Cannot leave either of your own kings in check.
// If both your kings are in check and you can't resolve both, it's mate.
class TwoKingGame : public Game {
public:
    TwoKingGame() { setupPieces(); }

    string getName() const override { return "Two-King Chess"; }

    void setupPieces() override {
        // White: two kings instead of king+queen
        board.placePiece(new Rook("white", 0, 0), 0, 0);
        board.placePiece(new Knight("white", 0, 1), 0, 1);
        board.placePiece(new Bishop("white", 0, 2), 0, 2);
        board.placePiece(new King("white", 0, 3), 0, 3);  // 2nd king (queen's spot)
        board.placePiece(new King("white", 0, 4), 0, 4);  // original king
        board.placePiece(new Bishop("white", 0, 5), 0, 5);
        board.placePiece(new Knight("white", 0, 6), 0, 6);
        board.placePiece(new Rook("white", 0, 7), 0, 7);
        for (int c = 0; c < 8; c++)
            board.placePiece(new Pawn("white", 1, c), 1, c);

        // Black: two kings instead of king+queen
        board.placePiece(new Rook("black", 7, 0), 7, 0);
        board.placePiece(new Knight("black", 7, 1), 7, 1);
        board.placePiece(new Bishop("black", 7, 2), 7, 2);
        board.placePiece(new King("black", 7, 3), 7, 3);  // 2nd king
        board.placePiece(new King("black", 7, 4), 7, 4);  // original king
        board.placePiece(new Bishop("black", 7, 5), 7, 5);
        board.placePiece(new Knight("black", 7, 6), 7, 6);
        board.placePiece(new Rook("black", 7, 7), 7, 7);
        for (int c = 0; c < 8; c++)
            board.placePiece(new Pawn("black", 6, c), 6, c);
    }

    // In two-king: you're in check if EITHER of your kings is attacked
    bool isInCheck(const string& color) override {
        string enemy = (color == "white") ? "black" : "white";
        auto kings = board.findPieces('K', color);
        for (Piece* king : kings) {
            if (board.isSquareAttackedBy(king->getRow(), king->getCol(), enemy))
                return true;
        }
        return false;
    }

    // Checkmate: in check AND no legal moves can resolve it
    bool isCheckmate(const string& color) override {
        return isInCheck(color) && getLegalMoves(color).empty();
    }

    // Stalemate: NOT in check but no legal moves
    bool isStalemate(const string& color) override {
        return !isInCheck(color) && getLegalMoves(color).empty();
    }

    // A move is legal if it doesn't leave EITHER of your own kings in check
    // Override the base class method for two-king logic
    bool doesMoveLeaveSelfInCheck(const Move& m, const string& color) {
        Piece* movingPiece = board.getPieceAt(m.fromR, m.fromC);
        if (!movingPiece) return true;

        Piece* captured = board.getPieceAt(m.toR, m.toC);
        int oldRow = movingPiece->getRow(), oldCol = movingPiece->getCol();

        // Temp move
        board.getSquare(m.toR, m.toC).setPiece(movingPiece);
        board.getSquare(m.fromR, m.fromC).clearPiece();
        movingPiece->setRow(m.toR);
        movingPiece->setCol(m.toC);
        if (captured) captured->setIsAlive(false);

        // En passant temp capture
        Piece* epCaptured = nullptr;
        if (movingPiece->getSymbol() == 'P' && m.toC != m.fromC && !captured) {
            int epR = board.getEnPassantRow(), epC = board.getEnPassantCol();
            if (m.toR == epR && m.toC == epC) {
                int epPawnRow = (color == "white") ? m.toR - 1 : m.toR + 1;
                epCaptured = board.getPieceAt(epPawnRow, m.toC);
                if (epCaptured) {
                    epCaptured->setIsAlive(false);
                    board.getSquare(epPawnRow, m.toC).clearPiece();
                }
            }
        }

        // Check if ANY of our kings are in check after the move
        bool inCheck = isInCheck(color);

        // Undo
        board.getSquare(m.fromR, m.fromC).setPiece(movingPiece);
        board.getSquare(m.toR, m.toC).setPiece(captured);
        movingPiece->setRow(oldRow);
        movingPiece->setCol(oldCol);
        if (captured) captured->setIsAlive(true);
        if (epCaptured) {
            int epPawnRow = (color == "white") ? m.toR - 1 : m.toR + 1;
            epCaptured->setIsAlive(true);
            board.getSquare(epPawnRow, m.toC).setPiece(epCaptured);
        }

        return inCheck;
    }

    vector<Move> getLegalMoves(const string& color) override {
        vector<Move> moves;
        for (Piece* p : board.getAllPieces()) {
            if (!p->getIsAlive() || p->getColor() != color) continue;
            auto pseudo = p->getPseudoLegalMoves(board);
            for (auto& [tr, tc] : pseudo) {
                Move m(p->getRow(), p->getCol(), tr, tc, p->getSymbol());
                if (!doesMoveLeaveSelfInCheck(m, color))
                    moves.push_back(m);
            }
        }
        // No castling in two-king chess (simplified — neither king castles)
        return moves;
    }

    bool executeMove(const Move& m) override {
        Piece* piece = board.getPieceAt(m.fromR, m.fromC);
        if (!piece) return false;

        bool isPawnMove = (piece->getSymbol() == 'P');
        bool isCapture = false;

        // En passant capture
        if (isPawnMove && m.toC != m.fromC &&
            !board.getPieceAt(m.toR, m.toC)) {
            int epPawnRow = (piece->getColor() == "white") ? m.toR - 1 : m.toR + 1;
            board.removePieceAt(epPawnRow, m.toC);
            isCapture = true;
            cout << "En passant capture!" << endl;
        }

        // Normal capture — in two-king, capturing a king wins!
        Piece* captured = board.getPieceAt(m.toR, m.toC);
        if (captured) {
            isCapture = true;
            if (captured->getSymbol() == 'K') {
                // King captured — game over!
                captured->setIsAlive(false);
                board.movePieceRaw(m.fromR, m.fromC, m.toR, m.toC);
                piece->setHasMoved(true);
                board.clearEnPassant();
                string w = (currentTurn == "white") ? "White" : "Black";
                board.printBoard();
                cout << w << " captures a King! " << w << " wins!" << endl;
                gameOver = true;
                winner = currentTurn;
                return true;
            }
            captured->setIsAlive(false);
            cout << currentTurn << " captures " << captured->getSymbol() << "!" << endl;
        }

        // Update en passant
        board.clearEnPassant();
        if (isPawnMove && abs(m.toR - m.fromR) == 2) {
            board.setEnPassant((m.fromR + m.toR) / 2, m.fromC);
        }

        // Move
        board.movePieceRaw(m.fromR, m.fromC, m.toR, m.toC);
        piece->setHasMoved(true);

        // Pawn promotion — promote to rook (no queen in this variant)
        if (isPawnMove) {
            int promoRow = (piece->getColor() == "white") ? 7 : 0;
            if (m.toR == promoRow) {
                Rook* rook = new Rook(piece->getColor(), m.toR, m.toC);
                rook->setHasMoved(true);
                board.getSquare(m.toR, m.toC).setPiece(rook);
                piece->setIsAlive(false);
                board.placePiece(rook, m.toR, m.toC);
                cout << "Pawn promoted to Rook!" << endl;
            }
        }

        updateHalfMoveClock(isPawnMove, isCapture);
        postMoveUpdate();
        return true;
    }
};
