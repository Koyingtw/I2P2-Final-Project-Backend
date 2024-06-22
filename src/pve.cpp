#include <iostream>
#include "pve.hpp"
#include "constants.hpp"
#include "ai.hpp"
#include "matrix.hpp"


Pve::Pve() {
    user1->board.resize(BOARD_HEIGHT, std::vector<char>(WIDTH, 'X'));
    user1->block.resize(BLOCK_HEIGHT, std::vector<char>(BLOCK_HEIGHT, 'X'));
}

Pve::Pve(User *u1, AI *ai) {
    this->user1 = u1;
    this->ai = ai;
    this->user1->board.resize(BOARD_HEIGHT, std::vector<char>(WIDTH, 'X'));
    this->ai->board.resize(BOARD_HEIGHT, std::vector<char>(WIDTH, 'X'));
    this->user1->block = stringToMatrix(generateBlock());
    this->ai->block = stringToMatrix(generateBlock());
}

std::string Pve::operation(server &m_server, websocketpp::connection_hdl &hdl, std::string input) {
    User *user = user1;
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

    std::cout << "dropRow: " << dropRow << std::endl;

    if (dropRow == -1) {
        m_server.send(user1->hdl, "Game over", websocketpp::frame::opcode::text);
        return "Game Over";
    }

    // 將方塊放置在盤面上
    placeBlock(user->board, user->block, dropRow, 0);

    // 印出盤面
    std::cout << "user: " << user << std::endl;
    // printMatrix(user->board);

    std::string ret = generateBlock();

    ret += '\n';

    for (int i = 0; i < BOARD_HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            ret += user1->board[i][j];
        }
        ret += '\n';
    }
    m_server.send(user1->hdl, std::to_string(2) + ret, websocketpp::frame::opcode::text);

    // AI

    std::string AIret = AI::genAnswer(ai->board, ai->block);

    for (int i = 0; i < BLOCK_HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            ai->block[i][j] = AIret[i * WIDTH + j];
        }
    }

    std::cout << "dropRow: " << dropRow << std::endl;
    dropRow = -1;
    for (int i = 0; i < BOARD_HEIGHT; ++i) {
        if (!canPlaceBlock(ai->board, ai->block, i, 0)) {
            break;
        }
        dropRow = i;
    }

    if (dropRow == -1) {
        m_server.send(user1->hdl, "Game over", websocketpp::frame::opcode::text);
        return "Game Over";
    }


    // 將方塊放置在盤面上
    placeBlock(ai->board, ai->block, dropRow, 0);

    std::string retAI = "0";
    for (int i = 0; i < BOARD_HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            retAI += ai->board[i][j];
        }
        retAI += '\n';
    }
    m_server.send(user1->hdl, retAI, websocketpp::frame::opcode::text);
    ai->block = stringToMatrix(generateBlock());

    return ret;
}