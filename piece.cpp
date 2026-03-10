#include <iostream>

using namespace std;

class Piece {
    
    string color;
    bool isAlive;

    public:
    Piece(string color, bool isAlive) {
        this->color = color;
        this->isAlive = isAlive;
    }

    string getColor() {
        return color;
    }

    bool getIsAlive() {
        return isAlive;
    }

    void setColor(string color) {
        this->color = color;
    }

    void setIsAlive(bool isAlive) {
        this->isAlive = isAlive;
    }


    // this makes sure the position is valid, not if the move is valid
    void setPosition(string Poposition) {

        //make sure it is a1 to h8
        if (position.length() != 2) {
            cout << "Invalid position" << endl;
            return;
        }
        if (position[0] < 'a' || position[0] > 'h') {
            cout << "Invalid position" << endl;
            return;
        }
        if (position[1] < '1' || position[1] > '8') {
            cout << "Invalid position" << endl;
            return;
        }
        this->position = position;
    }

    virtual void move() = 0;
    virtual void capture() = 0;
    virtual void die() = 0;
}