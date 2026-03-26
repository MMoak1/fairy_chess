#pragma once
#include <string>
#include <tuple>

using namespace std;

struct Move {
    int fromR, fromC, toR, toC;
    char pieceSymbol;

    Move() : fromR(0), fromC(0), toR(0), toC(0), pieceSymbol(' ') {}
    Move(int fr, int fc, int tr, int tc, char ps = ' ')
        : fromR(fr), fromC(fc), toR(tr), toC(tc), pieceSymbol(ps) {}

    bool operator==(const Move& o) const {
        return fromR == o.fromR && fromC == o.fromC && toR == o.toR && toC == o.toC;
    }

    // Convert to long algebraic string e.g. "Pe2e4"
    string toLAN() const {
        string s;
        s += pieceSymbol;
        s += (char)('a' + fromC);
        s += (char)('1' + fromR);
        s += (char)('a' + toC);
        s += (char)('1' + toR);
        return s;
    }

    // Parse from long algebraic string
    static bool parse(const string& input, Move& out) {
        if (input.length() != 5) return false;
        out.pieceSymbol = toupper(input[0]);
        out.fromC = input[1] - 'a';
        out.fromR = input[2] - '1';
        out.toC = input[3] - 'a';
        out.toR = input[4] - '1';
        return (out.fromR >= 0 && out.fromR <= 7 && out.fromC >= 0 && out.fromC <= 7 &&
                out.toR >= 0 && out.toR <= 7 && out.toC >= 0 && out.toC <= 7);
    }
};
