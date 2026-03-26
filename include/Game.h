#pragma once
#include "Board.h"
#include "Move.h"
#include <vector>
#include <string>
#include <iostream>

using namespace std;

// Abstract Game class — variant rules live here
class Game {
protected:
    Board board;
    string currentTurn;
    bool gameOver;
    string winner; // "" = no winner yet, "white"/"black"/"draw"
    int halfMoveClock; // for 50-move rule: resets on pawn move or capture

public:
    Game() : currentTurn("white"), gameOver(false), winner(""), halfMoveClock(0) {}
    virtual ~Game() {}

    Board& getBoard() { return board; }
    string getCurrentTurn() const { return currentTurn; }
    bool isGameOver() const { return gameOver; }
    string getWinner() const { return winner; }

    void switchTurn() { currentTurn = (currentTurn == "white") ? "black" : "white"; }

    // --- Pure virtual: variant-specific ---
    virtual string getName() const = 0;
    virtual void setupPieces() = 0;
    virtual bool isInCheck(const string& color) = 0;
    virtual bool isCheckmate(const string& color) = 0;
    virtual bool isStalemate(const string& color) = 0;

    // Returns all legal moves for a color (pseudo-legal filtered by legality)
    virtual vector<Move> getLegalMoves(const string& color) = 0;

    // Execute a raw move (assumes it's already validated as legal)
    // Handles captures, en passant, promotion, castling — variant specific
    virtual bool executeMove(const Move& move) = 0;

    // --- Shared: parse + validate + execute from user input ---
    bool parseMoveAndExecute(const string& input) {
        Move m;
        if (!Move::parse(input, m)) {
            cout << "Invalid format. Use: <piece><from><to> e.g. Pe2e4" << endl;
            return false;
        }
        Piece* piece = board.getPieceAt(m.fromR, m.fromC);
        if (!piece) {
            cout << "No piece at " << input[1] << input[2] << "." << endl;
            return false;
        }
        if (toupper(piece->getSymbol()) != m.pieceSymbol) {
            cout << "The piece at " << input[1] << input[2] << " is a "
                 << piece->getSymbol() << ", not a " << m.pieceSymbol << "." << endl;
            return false;
        }
        if (piece->getColor() != currentTurn) {
            cout << "It's " << currentTurn << "'s turn." << endl;
            return false;
        }

        // Check if this move is in the legal moves list
        auto legal = getLegalMoves(currentTurn);
        bool found = false;
        for (auto& lm : legal) {
            if (lm.fromR == m.fromR && lm.fromC == m.fromC &&
                lm.toR == m.toR && lm.toC == m.toC) {
                found = true;
                break;
            }
        }
        if (!found) {
            cout << "Illegal move." << endl;
            return false;
        }

        return executeMove(m);
    }

    // Call from executeMove: resets clock on pawn move or capture, else increments
    void updateHalfMoveClock(bool isPawnMove, bool isCapture) {
        if (isPawnMove || isCapture)
            halfMoveClock = 0;
        else
            halfMoveClock++;
    }

    int getHalfMoveClock() const { return halfMoveClock; }

    // --- Post-move: check for check/mate/stalemate/50-move, switch turns ---
    void postMoveUpdate() {
        switchTurn();

        // 50-move rule: 100 half-moves without pawn move or capture
        if (halfMoveClock >= 100) {
            board.printBoard();
            cout << "DRAW by 50-move rule!" << endl;
            gameOver = true;
            winner = "draw";
            return;
        }

        if (isCheckmate(currentTurn)) {
            string w = (currentTurn == "white") ? "Black" : "White";
            board.printBoard();
            cout << "CHECKMATE! " << w << " wins!" << endl;
            gameOver = true;
            winner = (currentTurn == "white") ? "black" : "white";
        } else if (isStalemate(currentTurn)) {
            board.printBoard();
            cout << "STALEMATE! The game is a draw." << endl;
            gameOver = true;
            winner = "draw";
        } else if (isInCheck(currentTurn)) {
            cout << currentTurn << " is in CHECK!" << endl;
        }
    }

    // Test if a move leaves the given color in check (for legality testing)
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

        // Handle en passant capture temporarily
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
};
