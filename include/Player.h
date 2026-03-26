#pragma once
#include "Game.h"
#include "Move.h"
#include <string>
#include <vector>

using namespace std;

// Abstract Player interface
class Player {
protected:
    string color;
    string name;

public:
    Player(const string& color, const string& name) : color(color), name(name) {}
    virtual ~Player() {}

    string getColor() const { return color; }
    string getName() const { return name; }

    // Given the game state, return a move. For human: prompt input. For AI: compute.
    virtual Move getMove(Game& game) = 0;
};
