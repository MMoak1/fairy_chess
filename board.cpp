#include <iostream>
#include <vector>
using namespace std;

class board {

    vector<vector<Square>> vec; 

    public:
    board() {
        vec.resize(8, vector<Square>(8));
    }

    void printBoard() {
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                cout << vec[i][j].getColor() << " ";
            }
            cout << endl;
        }
    }

    void setupBoard() {

         for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if (i % 2 == 0) {
                    if (j % 2 == 0) {
                        vec[i][j] = Square("white", false, Piece("white", true));
                    } else {
                        vec[i][j] = Square("black", false, Piece("black", true));
                    }
                } else {
                    if (j % 2 == 0) {
                        vec[i][j] = Square("black", false, Piece("black", true));
                    } else {
                        vec[i][j] = Square("white", false, Piece("white", true));
                    }
                }
            }
        }
        
    }

}