#pragma once
#include "Player.h"
#include <iostream>

using namespace std;

class HumanPlayer : public Player {
public:
    HumanPlayer(const string& color) : Player(color, "Human (" + color + ")") {}

    Move getMove(Game& game) override {
        while (true) {
            cout << "[" << color << "'s turn] Enter move: ";
            string input;
            getline(cin, input);

            if (input == "quit" || input == "exit") {
                // Signal quit with an invalid move
                return Move(-1, -1, -1, -1);
            }

            Move m;
            if (!Move::parse(input, m)) {
                cout << "Invalid format. Use: <piece><from><to> e.g. Pe2e4" << endl;
                continue;
            }

            Piece* piece = game.getBoard().getPieceAt(m.fromR, m.fromC);
            if (!piece) {
                cout << "No piece at " << input[1] << input[2] << "." << endl;
                continue;
            }
            if (toupper(piece->getSymbol()) != m.pieceSymbol) {
                cout << "The piece at " << input[1] << input[2] << " is a "
                     << piece->getSymbol() << ", not a " << m.pieceSymbol << "." << endl;
                continue;
            }
            if (piece->getColor() != color) {
                cout << "That's not your piece." << endl;
                continue;
            }

            // Check legality
            auto legal = game.getLegalMoves(color);
            for (auto& lm : legal) {
                if (lm.fromR == m.fromR && lm.fromC == m.fromC &&
                    lm.toR == m.toR && lm.toC == m.toC) {
                    return m;
                }
            }
            cout << "Illegal move." << endl;
        }
    }
};
