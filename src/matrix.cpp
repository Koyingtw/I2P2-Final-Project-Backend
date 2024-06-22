#include <iostream>
#include <vector>
#include <random>
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

std::string matrixToString(const Matrix& matrix) {
    std::string str;
    for (const auto& row : matrix) {
        for (char cell : row) {
            str += cell;
        }
        str += '\n';
    }
    return str;
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

int placeBlock(std::vector<std::vector<char>>& board, const std::vector<std::vector<char>>& block, int startRow, int startCol) {
    for (int i = 0; i < BLOCK_HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            if (block[i][j] != 'X') {
                board[startRow + i][startCol + j] = block[i][j];
            }
        }
    }

    int clearRow = 0;
    for (int i = BLOCK_HEIGHT - 1; ~i; i--) {
        bool clear = true;
        for (int j = 0; j < WIDTH; j++) {
            if (board[startRow + i][j] == 'X') {
                clear = false;
                break;
            }
        }
        if (clear) {
            clearRow++;
            for (int j = i; j; j--) {
                for (int k = 0; k < WIDTH; k++) {
                    board[startRow + j][k] = board[startRow + j - 1][k];
                }
            }
            for (int j = 0; j < WIDTH; j++) {
                board[startRow][j] = 'X';
            }
            i++;
        }
    }
    return clearRow;
}


std::string generateBlock() {
    std::string blocks[] = {I, L, J, O, S, T, Z};
    std::mt19937 rng(std::random_device{}());
    return blocks[rng() % 7];
}

char generateBlockID() {
    char blocks[] = {'I', 'L', 'J', 'O', 'S', 'T', 'Z'};
    std::mt19937 rng(std::random_device{}());
    return blocks[rng() % 7];
}

std::string generateBlockString(char blockID) {
    std::string ret;
    switch (blockID) {
        case 'I':
            ret = I;
            break;
        case 'J':
            ret = J;
            break;
        case 'L':
            ret = L;
            break;
        case 'O':
            ret = O;
            break;
        case 'S':
            ret = S;
            break;
        case 'T':
            ret = T;
            break;
        case 'Z':
            ret = Z;
            break;
    }
    return ret;
}

char transToBlockID(const Matrix& block) {
    for (int i = 0; i < BLOCK_HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            if (block[i][j] != 'X') {
                return block[i][j];
            }
        }
    }
    return 'X';
}