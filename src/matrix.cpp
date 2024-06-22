#include <iostream>
#include <vector>
#include "matrix.hpp"
#include "constants.hpp"

typedef std::vector<std::vector<char>> Matrix;

Matrix stringToMatrix(std::string str) {
    Matrix matrix;
    for (int i = 0; i < str.size() / (WIDTH + 1); ++i) {
        std::vector<char> row;
        for (int j = 0; j < WIDTH; ++j) {
            row.push_back(str[i * (WIDTH + 1) + j]);
        }
        matrix.push_back(row);
    }
    return matrix;
}

void printMatrix(const std::vector<std::vector<char>>& matrix) {
    for (const auto& row : matrix) {
        for (char cell : row) {
            // if (cell == 'X')
            //     std::cout << ' ';
            // else
            //     std::cout << cell;
            std::cout << cell;
        }
        std::cout << std::endl;
    }
}

bool canPlaceBlock(const std::vector<std::vector<char>>& board, const std::vector<std::vector<char>>& block, int startRow, int startCol) {
    for (int i = 0; i < BLOCK_HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            if (block[i][j] != 'X' && (startRow + i >= BOARD_HEIGHT || startCol + j >= WIDTH || board[startRow + i][startCol + j] != 'X')) {
                return false;
            }
        }
    }
    return true;
}

void placeBlock(std::vector<std::vector<char>>& board, const std::vector<std::vector<char>>& block, int startRow, int startCol) {
    for (int i = 0; i < BLOCK_HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            if (block[i][j] != 'X') {
                board[startRow + i][startCol + j] = block[i][j];
            }
        }
    }
}


std::string generateBlock() {
    std::string blocks[] = {I, L, J, O, S, T, Z};
    return blocks[rand() % 7];
}