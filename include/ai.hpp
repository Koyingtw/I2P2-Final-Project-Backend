#pragma once
#include <vector>
#include <string>

typedef std::vector<std::vector<char>> Matrix;


class AI {
    private:
        static bool shiftAble(const Matrix& board, const Matrix& block, int direction); // -: left, +: up
        static Matrix shift(const Matrix& block, int direction); // -: left, +: up
        static Matrix rotate(const char blockID, int direction);
        static int calculateClearedLines(const Matrix& board);
        static int calculateHoles(const Matrix& board);
        static int calculateMaxHeight(const Matrix& board);
        static int calculateBumpiness(const Matrix& board);
        static int evaluateBoard(const Matrix& board);
    public:
        std::vector<std::vector<char>> block;
        std::vector<std::vector<char>> board;
        static std::string genAnswer(const Matrix& board, const Matrix& block);
};
