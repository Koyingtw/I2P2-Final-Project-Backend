#include <iostream>
#include "pve.hpp"
#include "constants.hpp"
#include "ai.hpp"
#include "matrix.hpp"
#include "user.hpp"


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

    for (int i = 0; i < 3; i++) {
        char next = generateBlockID();
        this->user1->nextBlock.push_back(next);
        next = generateBlockID();
        this->ai->nextBlock.push_back(next);
    }

    this->user1->holdBlock = 'X';
}

void Pve::hold(server &m_server, websocketpp::connection_hdl &hdl) {
    User *user = user1;
    if (user->holdBlock == 'X') {
        user->holdBlock = transToBlockID(user->block);
        user->block = stringToMatrix(generateBlockString(user->nextBlock[0]));
        user->nextBlock.pop_front();
        user->nextBlock.push_back(generateBlockID());
    } else {
        char tmp = user->holdBlock;
        user->holdBlock = transToBlockID(user->block);
        user->block = stringToMatrix(generateBlockString(tmp));
    }

    std::string ret = "my-block\n" + matrixToString(user->block);


    ret += "my-hold\n";
    ret += user->holdBlock;
    ret += '\n';

    ret += "my-next\n";
    for (int i = 0; i < 3; i++) {
        ret += user->nextBlock[i];
    }
    ret += '\n';

    ret += "my-score " + std::to_string(user->score) + '\n';
    ret += "ai-score " + std::to_string(ai->score) + '\n';
    m_server.send(user1->hdl, ret, websocketpp::frame::opcode::text);
}

std::string Pve::operation(server &m_server, websocketpp::connection_hdl &hdl, std::string input) {
    User *user = user1;
    for (int i = 0; i < BLOCK_HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            user->block[i][j] = input[i * WIDTH + j + 1];
        }
    }

    int dropRow = -1;
    for (int i = 0; i < BOARD_HEIGHT; ++i) {
        if (!canPlaceBlock(user->board, user->block, i, 0)) {
            break;
        }
        dropRow = i;
    }


    if (dropRow == -1) {
        m_server.send(user1->hdl, "game-over", websocketpp::frame::opcode::text);
        return "Game Over";
    }

    // 將方塊放置在盤面上
    int clearRows = placeBlock(user->board, user->block, dropRow, 0);
    user->score += clearRows * 100;
    ai->score -= clearRows * 100;

    // 印出盤面
    // std::cout << "user: " << user << std::endl;
    // printMatrix(user->board);

    user->block = stringToMatrix(generateBlockString(user->nextBlock[0]));
    user->nextBlock.pop_front();
    user->nextBlock.push_back(generateBlockID());
    std::string ret = "my-block\n" + matrixToString(user->block);

    ret += "my-board\n";

    for (int i = 0; i < BOARD_HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            ret += user1->board[i][j];
        }
        ret += '\n';
    }

    ret += "my-hold\n";
    ret += user->holdBlock;
    ret += '\n';

    ret += "my-next\n";
    for (int i = 0; i < 3; i++) {
        ret += user->nextBlock[i];
    }
    ret += '\n';

    ret += "my-score " + std::to_string(user->score) + '\n';
    ret += "ai-score " + std::to_string(ai->score) + '\n';
    m_server.send(user1->hdl, ret, websocketpp::frame::opcode::text);

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
        m_server.send(user1->hdl, "you-win", websocketpp::frame::opcode::text);
        return "Game Over";
    }


    // 將方塊放置在盤面上
    clearRows = placeBlock(ai->board, ai->block, dropRow, 0);
    std::cout << "clearRows: " << clearRows << std::endl;
    ai->score += clearRows * 100;
    user1->score -= clearRows * 100;



    std::string retAI = "ai-block\n";

    retAI += generateBlockString(ai->nextBlock[0]);
    ai->block = stringToMatrix(generateBlockString(ai->nextBlock[0]));
    ai->nextBlock.pop_front();
    ai->nextBlock.push_back(generateBlockID());

    retAI += "ai-board\n";
    for (int i = 0; i < BOARD_HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            retAI += ai->board[i][j];
        }
        retAI += '\n';
    }

    retAI += "ai-next\n";
    for (int i = 0; i < 3; i++) {
        retAI += ai->nextBlock[i];
    }
    retAI += '\n';

    retAI += "my-score " + std::to_string(user->score) + '\n';
    retAI += "ai-score " + std::to_string(ai->score) + '\n';

    m_server.send(user1->hdl, retAI, websocketpp::frame::opcode::text);

    return ret;
}