#pragma once
#include "Player.h"
#include <random>
#include <iostream>

using namespace std;

class RandomAI : public Player {
    mt19937 rng;

public:
    RandomAI(const string& color) : Player(color, "Random AI (" + color + ")"), rng(random_device{}()) {}

    Move getMove(Game& game) override {
        auto legal = game.getLegalMoves(color);
        if (legal.empty()) {
            return Move(-1, -1, -1, -1); // no moves
        }
        uniform_int_distribution<int> dist(0, legal.size() - 1);
        Move chosen = legal[dist(rng)];
        cout << "[" << name << "] plays " << chosen.toLAN() << endl;
        return chosen;
    }
};
