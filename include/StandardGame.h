#pragma once
#include "Game.h"
#include "Pawn.h"
#include "Rook.h"
#include "Knight.h"
#include "Bishop.h"
#include "Queen.h"
#include "King.h"
#include <cmath>

using namespace std;

class StandardGame : public Game {
public:
    StandardGame() { setupPieces(); }

    string getName() const override { return "Standard Chess"; }

    void setupPieces() override {
        board.placePiece(new Rook("white", 0, 0), 0, 0);
        board.placePiece(new Knight("white", 0, 1), 0, 1);
        board.placePiece(new Bishop("white", 0, 2), 0, 2);
        board.placePiece(new Queen("white", 0, 3), 0, 3);
        board.placePiece(new King("white", 0, 4), 0, 4);
        board.placePiece(new Bishop("white", 0, 5), 0, 5);
        board.placePiece(new Knight("white", 0, 6), 0, 6);
        board.placePiece(new Rook("white", 0, 7), 0, 7);
        for (int c = 0; c < 8; c++)
            board.placePiece(new Pawn("white", 1, c), 1, c);

        board.placePiece(new Rook("black", 7, 0), 7, 0);
        board.placePiece(new Knight("black", 7, 1), 7, 1);
        board.placePiece(new Bishop("black", 7, 2), 7, 2);
        board.placePiece(new Queen("black", 7, 3), 7, 3);
        board.placePiece(new King("black", 7, 4), 7, 4);
        board.placePiece(new Bishop("black", 7, 5), 7, 5);
        board.placePiece(new Knight("black", 7, 6), 7, 6);
        board.placePiece(new Rook("black", 7, 7), 7, 7);
        for (int c = 0; c < 8; c++)
            board.placePiece(new Pawn("black", 6, c), 6, c);
    }

    bool isInCheck(const string& color) override {
        Piece* king = board.findFirstPiece('K', color);
        if (!king) return false;
        string enemy = (color == "white") ? "black" : "white";
        return board.isSquareAttackedBy(king->getRow(), king->getCol(), enemy);
    }

    bool isCheckmate(const string& color) override {
        return isInCheck(color) && getLegalMoves(color).empty();
    }

    bool isStalemate(const string& color) override {
        return !isInCheck(color) && getLegalMoves(color).empty();
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
            // Castling
            if (p->getSymbol() == 'K' && !p->getHasMoved()) {
                if (canCastleKingSide(color))
                    moves.push_back(Move(p->getRow(), 4, p->getRow(), 6, 'K'));
                if (canCastleQueenSide(color))
                    moves.push_back(Move(p->getRow(), 4, p->getRow(), 2, 'K'));
            }
        }
        return moves;
    }

    bool executeMove(const Move& m) override {
        Piece* piece = board.getPieceAt(m.fromR, m.fromC);
        if (!piece) return false;

        bool isPawnMove = (piece->getSymbol() == 'P');
        bool isCapture = false;

        // Castling
        if (piece->getSymbol() == 'K' && abs(m.toC - m.fromC) == 2) {
            bool kingSide = (m.toC > m.fromC);
            executeCastle(m.fromR, kingSide);
            updateHalfMoveClock(false, false);
            postMoveUpdate();
            return true;
        }

        // En passant capture
        if (isPawnMove && m.toC != m.fromC &&
            !board.getPieceAt(m.toR, m.toC)) {
            int epPawnRow = (piece->getColor() == "white") ? m.toR - 1 : m.toR + 1;
            board.removePieceAt(epPawnRow, m.toC);
            isCapture = true;
            cout << "En passant capture!" << endl;
        }

        // Normal capture
        Piece* captured = board.getPieceAt(m.toR, m.toC);
        if (captured) {
            captured->setIsAlive(false);
            isCapture = true;
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

        // Promotion
        if (isPawnMove) {
            int promoRow = (piece->getColor() == "white") ? 7 : 0;
            if (m.toR == promoRow) {
                Queen* queen = new Queen(piece->getColor(), m.toR, m.toC);
                queen->setHasMoved(true);
                board.getSquare(m.toR, m.toC).setPiece(queen);
                piece->setIsAlive(false);
                board.placePiece(queen, m.toR, m.toC);
                cout << "Pawn promoted to Queen!" << endl;
            }
        }

        updateHalfMoveClock(isPawnMove, isCapture);
        postMoveUpdate();
        return true;
    }

private:
    bool canCastleKingSide(const string& color) {
        int row = (color == "white") ? 0 : 7;
        Piece* king = board.getPieceAt(row, 4);
        Piece* rook = board.getPieceAt(row, 7);
        if (!king || !rook) return false;
        if (king->getSymbol() != 'K' || rook->getSymbol() != 'R') return false;
        if (king->getHasMoved() || rook->getHasMoved()) return false;
        if (king->getColor() != color || rook->getColor() != color) return false;
        if (!board.getSquare(row, 5).isEmpty() || !board.getSquare(row, 6).isEmpty()) return false;
        string enemy = (color == "white") ? "black" : "white";
        if (board.isSquareAttackedBy(row, 4, enemy)) return false;
        if (board.isSquareAttackedBy(row, 5, enemy)) return false;
        if (board.isSquareAttackedBy(row, 6, enemy)) return false;
        return true;
    }

    bool canCastleQueenSide(const string& color) {
        int row = (color == "white") ? 0 : 7;
        Piece* king = board.getPieceAt(row, 4);
        Piece* rook = board.getPieceAt(row, 0);
        if (!king || !rook) return false;
        if (king->getSymbol() != 'K' || rook->getSymbol() != 'R') return false;
        if (king->getHasMoved() || rook->getHasMoved()) return false;
        if (king->getColor() != color || rook->getColor() != color) return false;
        if (!board.getSquare(row, 1).isEmpty() || !board.getSquare(row, 2).isEmpty() ||
            !board.getSquare(row, 3).isEmpty()) return false;
        string enemy = (color == "white") ? "black" : "white";
        if (board.isSquareAttackedBy(row, 4, enemy)) return false;
        if (board.isSquareAttackedBy(row, 3, enemy)) return false;
        if (board.isSquareAttackedBy(row, 2, enemy)) return false;
        return true;
    }

    void executeCastle(int row, bool kingSide) {
        if (kingSide) {
            board.movePieceRaw(row, 4, row, 6);
            board.movePieceRaw(row, 7, row, 5);
            board.getPieceAt(row, 6)->setHasMoved(true);
            board.getPieceAt(row, 5)->setHasMoved(true);
            cout << "King-side castle!" << endl;
        } else {
            board.movePieceRaw(row, 4, row, 2);
            board.movePieceRaw(row, 0, row, 3);
            board.getPieceAt(row, 2)->setHasMoved(true);
            board.getPieceAt(row, 3)->setHasMoved(true);
            cout << "Queen-side castle!" << endl;
        }
        board.clearEnPassant();
    }
};
