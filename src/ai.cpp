#include <iostream>
#include <vector>
#include <algorithm>
#include <array>
#include "ai.hpp"
#include "constants.hpp"
#include "game.hpp"

typedef std::vector<std::vector<char>> Matrix;

bool AI::shiftAble(const Matrix& board, const Matrix& block, int direction) { // -: left, +: up
    for (int i = 0; i < BLOCK_HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            if (block[i][j] != 'X') {
                if (j + direction < 0 || j + direction >= WIDTH) {
                    return false;
                }
            }
        }
    }
    return true;
}

Matrix AI::shift(const Matrix& block, int direction) { // -: left, +: up
    Matrix newBlock(BLOCK_HEIGHT, std::vector<char>(WIDTH, 'X'));
    for (int i = 0; i < BLOCK_HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            if (block[i][j] != 'X')
                newBlock[i][j + direction] = block[i][j];
        }
    }
    return newBlock;
}

Matrix AI::rotate(const char blockID, int direction) {
    std::string ret;
    int index = (direction + 4) % 4;
    switch (blockID) {
        case 'I':
            ret = Is[index];
            break;
        case 'J':
            ret = Js[index];
            break;
        case 'L':
            ret = Ls[index];
            break;
        case 'O':
            ret = Os[index];
            break;
        case 'S':
            ret = Ss[index];
            break;
        case 'T':
            ret = Ts[index];
            break;
        case 'Z':
            ret = Zs[index];
            break;
    }

    Matrix newBlock(BLOCK_HEIGHT, std::vector<char>(WIDTH, 'X'));
    for (int i = 0; i < BLOCK_HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            newBlock[i][j] = ret[i * (WIDTH + 1) + j];
        }
    }

    return newBlock;
}

int AI::calculateClearedLines(const Matrix& board) {
    int clearedLines = 0;
    for (const auto& row : board) {
        if (std::all_of(row.begin(), row.end(), [](char cell) { return cell != 'X'; })) {
            clearedLines++;
        }
    }
    return clearedLines;
}

int AI::calculateHoles(const Matrix& board) {
    int holes = 0;
    for (int col = 0; col < WIDTH; ++col) {
        bool blockFound = false;
        for (int row = 0; row < BOARD_HEIGHT; ++row) {
            if (board[row][col] != 'X') {
                blockFound = true;
            } else if (blockFound) {
                holes++;
            }
        }
    }
    return holes;
}

int AI::calculateMaxHeight(const Matrix& board) {
    for (int row = 0; row < BOARD_HEIGHT; ++row) {
        if (std::any_of(board[row].begin(), board[row].begin() + WIDTH, [](char cell) { return cell != 'X'; })) {
            return BOARD_HEIGHT - row;
        }
    }
    return 0;
}

int AI::calculateBumpiness(const Matrix& board) {
    std::vector<int> heights(WIDTH, 0);
    for (int col = 0; col < WIDTH; ++col) {
        for (int row = 0; row < BOARD_HEIGHT; ++row) {
            if (board[row][col] != 'X') {
                heights[col] = BOARD_HEIGHT - row;
                break;
            }
        }
    }
    int bumpiness = 0;
    for (int col = 0; col < WIDTH - 1; ++col) {
        bumpiness += std::abs(heights[col] - heights[col + 1]);
    }
    return bumpiness;
}

int AI::evaluateBoard(const Matrix& board) {
    int clearedLines = calculateClearedLines(board);
    int holes = calculateHoles(board);
    int maxHeight = calculateMaxHeight(board);
    int bumpiness = calculateBumpiness(board);

    int score = (clearedLines * 100) - (holes * 50) - (maxHeight * 10) - (bumpiness * 5);
    // std::cout << "clearedLines: " << clearedLines << std::endl;
    // std::cout << "holes: " << holes << std::endl;
    // std::cout << "maxHeight: " << maxHeight << std::endl;
    // std::cout << "bumpiness: " << bumpiness << std::endl;
    // std::cout << "score: " << score << std::endl;
    return score;
}

std::string AI::genAnswer(const Matrix& board, const Matrix& block) {
    char blockID;
    printMatrix(block);
    for (int i = 0; i < BLOCK_HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            if (block[i][j] != 'X') {
                blockID = block[i][j];
                break;
            }
        }
    }

    std::cout << "blockID: " << blockID << std::endl;
    
    // enumerate all possible position and rotate

    std::array<int, 3> answer;
    answer[0] = -1e9;

    for (int position = -10; position <= 10; ++position) if (shiftAble(board, block, position)) {
        std::cout << "position: " << position << std::endl;
        for (int rotation = 0; rotation < 4; ++rotation) {
            // create board preview board after placing the block
            auto tmpBlock = rotate(blockID, rotation);
            auto tmpBoard = board;
            if (!shiftAble(tmpBoard, tmpBlock, position))
                continue;
            tmpBlock = shift(tmpBlock, position);

            int dropRow = -1;
            for (int i = 0; i < BOARD_HEIGHT; ++i) {
                if (!canPlaceBlock(tmpBoard, tmpBlock, i, 0)) {
                    break;
                }
                dropRow = i;
            }

            std::cout << "dropRow: " << dropRow << std::endl;

            placeBlock(tmpBoard, tmpBlock, dropRow, 0);


            // calculate the score
            int score = evaluateBoard(tmpBoard);
            if (score > answer[0]) {
                answer[0] = score;
                answer[1] = position;
                answer[2] = rotation;
            }
        }
    }

    auto bestBlock = rotate(blockID, answer[2]);
    bestBlock = shift(bestBlock, answer[1]);
    std::cout << "bestBlock: " << answer[0] << std::endl;
    printMatrix(bestBlock);
    return std::string(bestBlock[0].begin(), bestBlock[0].end()) + 
           std::string(bestBlock[1].begin(), bestBlock[1].end()) + 
           std::string(bestBlock[2].begin(), bestBlock[2].end()) + 
           std::string(bestBlock[3].begin(), bestBlock[3].end());
}