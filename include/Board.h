#pragma once
#include "Square.h"
#include "Piece.h"
#include "Move.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;

// Board is a dumb 8x8 grid container. No game rules live here.
class Board {
    Square grid[8][8];
    vector<Piece*> allPieces; // owns all piece memory
    int epRow, epCol; // en passant target square (-1 if none)

public:
    Board() : epRow(-1), epCol(-1) {
        for (int r = 0; r < 8; r++)
            for (int c = 0; c < 8; c++)
                grid[r][c].setColor(((r + c) % 2 == 0) ? "white" : "black");
    }

    ~Board() { for (Piece* p : allPieces) delete p; }

    // --- Grid access ---
    Square& getSquare(int r, int c) { return grid[r][c]; }
    bool isInBounds(int r, int c) const { return r >= 0 && r <= 7 && c >= 0 && c <= 7; }

    Piece* getPieceAt(int r, int c) const {
        if (r < 0 || r > 7 || c < 0 || c > 7) return nullptr;
        return grid[r][c].getPiece();
    }

    // --- Piece management ---
    void placePiece(Piece* p, int r, int c) {
        grid[r][c].setPiece(p);
        allPieces.push_back(p);
    }

    void movePieceRaw(int fromR, int fromC, int toR, int toC) {
        Piece* p = getPieceAt(fromR, fromC);
        if (!p) return;
        grid[toR][toC].setPiece(p);
        grid[fromR][fromC].clearPiece();
        p->setPosition(toR, toC);
    }

    void removePieceAt(int r, int c) {
        Piece* p = getPieceAt(r, c);
        if (p) {
            p->setIsAlive(false);
            grid[r][c].clearPiece();
        }
    }

    const vector<Piece*>& getAllPieces() const { return allPieces; }

    // --- En passant tracking (board state, not game logic) ---
    int getEnPassantRow() const { return epRow; }
    int getEnPassantCol() const { return epCol; }
    void setEnPassant(int r, int c) { epRow = r; epCol = c; }
    void clearEnPassant() { epRow = -1; epCol = -1; }

    // --- Attack queries (pure board geometry, no game rules) ---
    bool isSquareAttackedBy(int r, int c, const string& attackerColor) {
        for (Piece* p : allPieces) {
            if (p->getIsAlive() && p->getColor() == attackerColor) {
                if (p->isValidMove(*this, r, c))
                    return true;
            }
        }
        return false;
    }

    // --- Find pieces by type/color ---
    vector<Piece*> findPieces(char symbol, const string& color) const {
        vector<Piece*> result;
        for (Piece* p : allPieces) {
            if (p->getIsAlive() && p->getSymbol() == symbol && p->getColor() == color)
                result.push_back(p);
        }
        return result;
    }

    Piece* findFirstPiece(char symbol, const string& color) const {
        for (Piece* p : allPieces) {
            if (p->getIsAlive() && p->getSymbol() == symbol && p->getColor() == color)
                return p;
        }
        return nullptr;
    }

    // --- Display ---
    void printBoard() const {
        cout << endl;
        cout << "    a   b   c   d   e   f   g   h" << endl;
        cout << "  +---+---+---+---+---+---+---+---+" << endl;
        for (int r = 7; r >= 0; r--) {
            cout << (r + 1) << " |";
            for (int c = 0; c < 8; c++) {
                Piece* p = grid[r][c].getPiece();
                if (p) cout << " " << p->getDisplayChar() << " |";
                else   cout << "   |";
            }
            cout << " " << (r + 1) << endl;
            cout << "  +---+---+---+---+---+---+---+---+" << endl;
        }
        cout << "    a   b   c   d   e   f   g   h" << endl;
        cout << endl;
    }
};
