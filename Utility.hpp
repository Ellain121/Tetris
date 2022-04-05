#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <vector>
#include <string>

enum Keys
{
    Key_Down = 0,
    Key_Left,
    Key_Right,
    Key_Space
};

enum TetrisFigures
{
    I_tetr,
    O_tetr,
    T_tetr,
    J_tetr,
    L_tetr,
    S_tetr,
    Z_tetr
};

std::vector<std::vector<int>> createTetrisFigures()
{
    std::vector<std::vector<int>> tetFigures;
    std::vector<int>  I(4);
    std::vector<int>  J(4);
    std::vector<int>  L(4);
    std::vector<int>  O(4);
    std::vector<int>  S(4);
    std::vector<int>  T(4);
    std::vector<int>  Z(4);

    I[0] = 0b1111000000000000;
    I[0] = 0b1111000000000000;
    I[1] = 0b1000100010001000;
    I[2] = 0b1111000000000000;
    I[3] = 0b1000100010001000;
    tetFigures.push_back(I);

    J[0] = 0b1110001000000000;
    J[1] = 0b0100010011000000;
    J[2] = 0b1000111000000000;
    J[3] = 0b1100100010000000;
    tetFigures.push_back(J);

    L[0] = 0b1110100000000000;
    L[1] = 0b1100010001000000;
    L[2] = 0b0010111000000000;
    L[3] = 0b1000100011000000;
    tetFigures.push_back(L);

    O[0] = 0b1100110000000000;
    O[1] = 0b1100110000000000;
    O[2] = 0b1100110000000000;
    O[3] = 0b1100110000000000;
    tetFigures.push_back(O);

    S[0] = 0b0110110000000000;
    S[1] = 0b1000110001000000;
    S[2] = 0b0110110000000000;
    S[3] = 0b1000110001000000;
    tetFigures.push_back(S);

    T[0] = 0b1110010000000000;
    T[1] = 0b0100110001000000;
    T[2] = 0b0100111000000000;
    T[3] = 0b1000110010000000;
    tetFigures.push_back(T);

    Z[0] = 0b1100011000000000;
    Z[1] = 0b0100110010000000;
    Z[2] = 0b1100011000000000;
    Z[3] = 0b0100110010000000;
    tetFigures.push_back(Z);

    return tetFigures;
}

#endif