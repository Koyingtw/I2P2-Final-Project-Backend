#include "game.hpp"
#include "constants.hpp"
#include <iostream>


Game::Game() {
    user1->board.resize(BOARD_HEIGHT, std::vector<char>(WIDTH, 'X'));
    user2->board.resize(BOARD_HEIGHT, std::vector<char>(WIDTH, 'X'));
    user1->block.resize(BLOCK_HEIGHT, std::vector<char>(BLOCK_HEIGHT, 'X'));
    user2->block.resize(BLOCK_HEIGHT, std::vector<char>(BLOCK_HEIGHT, 'X'));
}

Game::Game(User *u1, User *u2) {
    this->user1 = u1;
    this->user2 = u2;
    this->user1->board.resize(BOARD_HEIGHT, std::vector<char>(WIDTH, 'X'));
    this->user2->board.resize(BOARD_HEIGHT, std::vector<char>(WIDTH, 'X'));
    this->user1->block.resize(BLOCK_HEIGHT, std::vector<char>(BLOCK_HEIGHT, 'X'));
    this->user2->block.resize(BLOCK_HEIGHT, std::vector<char>(BLOCK_HEIGHT, 'X'));
}

void printMatrix(const std::vector<std::vector<char>>& matrix) {
    for (const auto& row : matrix) {
        for (char cell : row) {
            if (cell == 'X')
                std::cout << ' ';
            else
                std::cout << cell;
            // cout << cell;
        }
        std::cout << std::endl;
    }
}

bool canPlaceBlock(const std::vector<std::vector<char>>& board, const std::vector<std::vector<char>>& block, int startRow, int startCol) {
    for (int i = 0; i < BLOCK_HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            if (block[i][j] != 'X' && board[startRow + i][startCol + j] != 'X') {
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
    const std::string blocks[] = {I, L, J, O, S, T, Z};
    return blocks[rand() % 7];
}

std::string Game::operation(websocketpp::connection_hdl &hdl, std::string input) {
    User *user = (!(user1->hdl).owner_before(hdl) && !hdl.owner_before(user1->hdl)) ? user1 : user2;
    std::cout << "user: " << user << std::endl;
    std::cout << "user1: " << &user1 << std::endl;
    std::cout << "user2: " << &user2 << std::endl;
    for (int i = 0; i < BLOCK_HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            user->block[i][j] = input[i * WIDTH + j + 1];
        }
    }

    int dropRow = 0;
    for (int i = 0; i <= BOARD_HEIGHT - BLOCK_HEIGHT; ++i) {
        if (!canPlaceBlock(user->board, user->block, i, 0)) {
            break;
        }
        dropRow = i;
    }

    if (dropRow == 0) {
        return "Game Over";
    }

    // 將方塊放置在盤面上
    placeBlock(user->board, user->block, dropRow, 0);

    // 印出盤面
    // printMatrix(user.board);

    std::string ret = generateBlock();

    ret += '\n';

    for (int i = 0; i < BOARD_HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            ret += user1->board[i][j];
        }
        ret += '\n';
    }
    ret += '\n';

    for (int i = 0; i < BOARD_HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            ret += user2->board[i][j];
        }
        ret += '\n';
    }

    return ret;
}