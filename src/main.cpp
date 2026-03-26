#include <iostream>
#include <string>
#include <memory>
#include "StandardGame.h"
#include "TwoKingGame.h"
#include "HumanPlayer.h"
#include "RandomAI.h"

using namespace std;

int main() {
    cout << "==========================================" << endl;
    cout << "       FAIRY CHESS - Terminal Edition      " << endl;
    cout << "==========================================" << endl;
    cout << endl;

    // Game selection
    cout << "Select game variant:" << endl;
    cout << "  1. Standard Chess" << endl;
    cout << "  2. Two-King Chess" << endl;
    cout << "Choice: ";
    string choice;
    getline(cin, choice);

    unique_ptr<Game> game;
    if (choice == "2") {
        game = make_unique<TwoKingGame>();
    } else {
        game = make_unique<StandardGame>();
    }

    cout << endl << "Playing: " << game->getName() << endl;

    // Player selection
    cout << endl << "Select opponent:" << endl;
    cout << "  1. Human vs Human" << endl;
    cout << "  2. Human (white) vs Random AI (black)" << endl;
    cout << "  3. Random AI (white) vs Human (black)" << endl;
    cout << "  4. Random AI vs Random AI" << endl;
    cout << "Choice: ";
    getline(cin, choice);

    unique_ptr<Player> whitePlayer, blackPlayer;
    if (choice == "2") {
        whitePlayer = make_unique<HumanPlayer>("white");
        blackPlayer = make_unique<RandomAI>("black");
    } else if (choice == "3") {
        whitePlayer = make_unique<RandomAI>("white");
        blackPlayer = make_unique<HumanPlayer>("black");
    } else if (choice == "4") {
        whitePlayer = make_unique<RandomAI>("white");
        blackPlayer = make_unique<RandomAI>("black");
    } else {
        whitePlayer = make_unique<HumanPlayer>("white");
        blackPlayer = make_unique<HumanPlayer>("black");
    }

    cout << endl;
    cout << "Long-form algebraic notation: <piece><from><to>" << endl;
    cout << "  Example: Pe2e4, Nb1c3, Ke1g1" << endl;
    cout << "  Pieces: P=Pawn, R=Rook, N=Knight, B=Bishop, Q=Queen, K=King" << endl;
    cout << "  Type 'quit' to exit." << endl;

    // Game loop
    while (!game->isGameOver()) {
        game->getBoard().printBoard();

        Player* current = (game->getCurrentTurn() == "white") ? whitePlayer.get() : blackPlayer.get();
        Move move = current->getMove(*game);

        // Quit signal
        if (move.fromR == -1) {
            cout << "Game ended by player." << endl;
            break;
        }

        game->executeMove(move);
    }

    return 0;
}