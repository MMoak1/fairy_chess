#include <iostream>
#include <string>

using namespace std;

class Square {
    
    string color;
    bool isOccupied;
    Piece piece;

    public:
    Square(string color, bool isOccupied, Piece piece) {
        this->color = color;
        this->isOccupied = isOccupied;
        this->piece = piece;
    }

    string getColor() {
        return color;
    }

    bool getIsOccupied() {
        return isOccupied;
    }

    Piece getPiece() {
        return piece;
    }

    void setColor(string color) {
        this->color = color;
    }

    void setIsOccupied(bool isOccupied) {
        this->isOccupied = isOccupied;
    }

    void setPiece(Piece piece) {
        this->piece = piece;
    }
}