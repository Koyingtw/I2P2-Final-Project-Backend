#include <iostream>
#include "game.hpp"
#include "constants.hpp"
#include "matrix.hpp"


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
    this->user1->block = stringToMatrix(generateBlock());
    this->user2->block = stringToMatrix(generateBlock());
}


std::string Game::operation(server &m_server, websocketpp::connection_hdl &hdl, std::string input) {
    User *user = (!(user1->hdl).owner_before(hdl) && !hdl.owner_before(user1->hdl)) ? user1 : user2;
    std::cout << "user: " << user << std::endl;
    std::cout << "user1: " << &user1 << std::endl;
    std::cout << "user2: " << &user2 << std::endl;
    for (int i = 0; i < BLOCK_HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            user->block[i][j] = input[i * WIDTH + j + 1];
        }
    }

    int dropRow = -1;
    for (int i = 0; i <= BOARD_HEIGHT; ++i) {
        if (!canPlaceBlock(user->board, user->block, i, 0)) {
            break;
        }
        dropRow = i;
    }

    if (dropRow == -1) {
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

    m_server.send(user1->hdl, std::to_string(user == user1) + ret, websocketpp::frame::opcode::text);
    m_server.send(user2->hdl, std::to_string(user == user2) + ret, websocketpp::frame::opcode::text);


    return ret;
}