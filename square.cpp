#include <iostream>

class Square {
    
    string color;
    Boolean isOccupied;
    Piece piece;

    public:
    Square(string color, Boolean isOccupied, Piece piece) {
        this->color = color;
        this->isOccupied = isOccupied;
        this->piece = piece;
    }

    string getColor() {
        return color;
    }

    Boolean getIsOccupied() {
        return isOccupied;
    }

    Piece getPiece() {
        return piece;
    }

    void setColor(string color) {
        this->color = color;
    }

    void setIsOccupied(Boolean isOccupied) {
        this->isOccupied = isOccupied;
    }

    void setPiece(Piece piece) {
        this->piece = piece;
    }
}