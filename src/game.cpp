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

    for (int i = 0; i < 3; i++) {
        char next = generateBlockID();
        this->user1->nextBlock.push_back(next);
        next = generateBlockID();
        this->user2->nextBlock.push_back(next);
    }

    this->user1->holdBlock = 'X';
    this->user2->holdBlock = 'X';
}

void Game::hold(server &m_server, websocketpp::connection_hdl &hdl) {
    User *user = (!(user1->hdl).owner_before(hdl) && !hdl.owner_before(user1->hdl)) ? user1 : user2;
    User *enemy = (user == user1) ? user2 : user1;
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
    ret += "enemy-score " + std::to_string(enemy->score) + '\n';
    m_server.send(user->hdl, ret, websocketpp::frame::opcode::text);

    ret = "enemy-block\n" + matrixToString(enemy->block);
    ret += "enemy-hold\n";
    ret += enemy->holdBlock;
    ret += '\n';

    ret += "enemy-next\n";
    for (int i = 0; i < 3; i++) {
        ret += enemy->nextBlock[i];
    }
    ret += '\n';

    ret += "my-score " + std::to_string(enemy->score) + '\n';
    ret += "enemy-score " + std::to_string(user->score) + '\n';
    m_server.send(enemy->hdl, ret, websocketpp::frame::opcode::text);
}



std::string Game::operation(server &m_server, websocketpp::connection_hdl &hdl, std::string input) {
    User *user = (!(user1->hdl).owner_before(hdl) && !hdl.owner_before(user1->hdl)) ? user1 : user2;
    User *enemy = (user == user1) ? user2 : user1;
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
        m_server.send(user->hdl, "Game over", websocketpp::frame::opcode::text);
        m_server.send(enemy->hdl, "You win", websocketpp::frame::opcode::text);
        return "Game Over";
    }

    // 將方塊放置在盤面上
    int clearRows = placeBlock(user->board, user->block, dropRow, 0);
    user->score += clearRows * 100;
    enemy->score -= clearRows * 100;

    // 印出盤面
    // printMatrix(user.board);

    user->block = stringToMatrix(generateBlockString(user->nextBlock[0]));
    user->nextBlock.pop_front();
    user->nextBlock.push_back(generateBlockID());
    std::string ret = "my-block\n" + matrixToString(user->block);

    ret += "my-board\n";

    for (int i = 0; i < BOARD_HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            ret += user->board[i][j];
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
    ret += "enemy-score " + std::to_string(enemy->score) + '\n';
    m_server.send(user->hdl, ret, websocketpp::frame::opcode::text);

    ret = "enemy-block\n" + matrixToString(user->block);

    ret += "enemy-board\n";
    for (int i = 0; i < BOARD_HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            ret += user->board[i][j];
        }
        ret += '\n';
    }

    ret += "enemy-hold\n";
    ret += user->holdBlock;
    ret += '\n';

    ret += "enemy-next\n";
    for (int i = 0; i < 3; i++) {
        ret += user->nextBlock[i];
    }
    ret += '\n';

    ret += "my-score " + std::to_string(user->score) + '\n';
    ret += "enemy-score " + std::to_string(enemy->score) + '\n';
    m_server.send(enemy->hdl, ret, websocketpp::frame::opcode::text);
    return "OK";
}