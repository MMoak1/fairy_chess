#pragma once
#include <string>

using namespace std;

class Piece; // forward declaration

class Square {
    string color;
    Piece* piece;

public:
    Square() : color(""), piece(nullptr) {}

    Square(string color) : color(color), piece(nullptr) {}

    string getColor() const { return color; }
    Piece* getPiece() const { return piece; }
    bool isEmpty() const { return piece == nullptr; }

    void setColor(string c) { color = c; }
    void setPiece(Piece* p) { piece = p; }
    void clearPiece() { piece = nullptr; }
};
