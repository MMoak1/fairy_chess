#include <iostream>

using namespace std;

class King : public Piece {

    string position;
    string color;
    bool isAlive;

    public:
    King(string position, string color, bool isAlive) : Piece(color, isAlive) {
        this->position = position;
    }

    string getPosition() {
        return position;
    }


    void move(string potentialPosition) {
        
    }

    void capture() {
        cout << "Pawn captured" << endl;
    }

    void die() {
        cout << "Pawn died" << endl;
    }

    void isValidMove(string potentialPosition)
    {

        // four conditions
        // 1. first move
        // 2. normal move
        // 3. capture
        // 4. en passant

        //normal move check
        
        //TODO
        
    }
}