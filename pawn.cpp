#include <iostream>

using namespace std;

class Pawn : public Piece {

    string position;
    string color;
    bool isAlive;

    public:
    Pawn(string position, string color, bool isAlive, bool isfirstMove) : Piece(color, isAlive) {
        this->position = position;
        this->isfirstMove = isfirstMove;
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