#include <iostream>

using namespace std;

class Queen : public Piece {

    string position;
    string color;
    bool isAlive;

    public:
    Queen(string position, string color, bool isAlive) : Piece(color, isAlive) {
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
        // 1. normal move
        // 2. capture

        //normal move check
        
        
    }
}