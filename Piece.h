#pragma once
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Board; // forward declaration

class Piece {
protected:
    string color;
    bool isAlive;
    int row, col;
    bool hasMoved;
    char symbol; // P, R, N, B, Q, K

public:
    Piece(string color, bool isAlive, int row, int col, char symbol)
        : color(color), isAlive(isAlive), row(row), col(col), hasMoved(false), symbol(symbol) {}

    virtual ~Piece() {}

    string getColor() const { return color; }
    bool getIsAlive() const { return isAlive; }
    int getRow() const { return row; }
    int getCol() const { return col; }
    bool getHasMoved() const { return hasMoved; }
    char getSymbol() const { return symbol; }

    void setColor(string c) { color = c; }
    void setIsAlive(bool alive) { isAlive = alive; }
    void setRow(int r) { row = r; }
    void setCol(int c) { col = c; }
    void setHasMoved(bool moved) { hasMoved = moved; }

    void setPosition(int r, int c) {
        if (r < 0 || r > 7 || c < 0 || c > 7) {
            cout << "Invalid position" << endl;
            return;
        }
        row = r;
        col = c;
    }

    // Returns the display character: uppercase for white, lowercase for black
    char getDisplayChar() const {
        if (color == "white") return symbol;
        return (char)tolower(symbol);
    }

    // Pure virtual: returns true if this piece can move from its current position to (toRow, toCol)
    // Does NOT check if the move leaves own king in check (Board handles that)
    virtual bool isValidMove(Board& board, int toRow, int toCol) = 0;

    // Get all pseudo-legal moves (before check filtering)
    virtual vector<pair<int,int>> getPseudoLegalMoves(Board& board) = 0;
};
