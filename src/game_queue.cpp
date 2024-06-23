#include "game_queue.hpp"
#include "game.hpp"
#include "ai.hpp"
#include "pve.hpp"
#include "matrix.hpp"

GameQueue::GameQueue() {}

void GameQueue::push(server &m_server, websocketpp::connection_hdl &hdl) {
    user_queue.push(hdl);

    if (user_queue.size() >= 2) {
        // 取出前兩個連線
        auto hdl1 = user_queue.front();
        user_queue.pop();
        auto hdl2 = user_queue.front();
        user_queue.pop();

        // 通知兩個連線已經配對成功
        m_server.send(hdl1, "Matched", websocketpp::frame::opcode::text);
        m_server.send(hdl2, "Matched", websocketpp::frame::opcode::text);

        // 開始遊戲
        User *user1 = new User;
        User *user2 = new User;
        user1->hdl = hdl1;
        user2->hdl = hdl2;
        // return;
        Game *game = new Game(user1, user2);
        games[hdl1] = game;
        games[hdl2] = game;


        in_game.insert(hdl1);
        in_game.insert(hdl2);

        // 傳送初始狀態
        // user 1
        std::string ret = "my-block\n" + matrixToString(user1->block);
        ret += "my-board\n" + matrixToString(user1->board);

        ret += "my-hold\n";
        ret += user1->holdBlock;
        ret += '\n';

        ret += "my-next\n";
        for (int i = 0; i < 3; i++) {
            ret += user1->nextBlock[i];
        }
        ret += '\n';

        ret += "enemy-block\n" + matrixToString(user2->block);
        ret += "enemy-board\n" + matrixToString(user2->board);
        ret += "enemy-hold\n";
        ret += user2->holdBlock;
        ret += '\n';

        ret += "enemy-next\n";
        for (int i = 0; i < 3; i++) {
            ret += user2->nextBlock[i];
        }
        ret += '\n';

        ret += "my-score " + std::to_string(user1->score) + '\n';
        ret += "enemy-score " + std::to_string(user2->score) + '\n';
        m_server.send(hdl1, ret, websocketpp::frame::opcode::text);

        ret = "my-block\n" + matrixToString(user2->block);
        ret += "my-board\n" + matrixToString(user2->board);

        ret += "my-hold\n";
        ret += user2->holdBlock;
        ret += '\n';

        ret += "my-next\n";
        for (int i = 0; i < 3; i++) {
            ret += user2->nextBlock[i];
        }
        ret += '\n';

        ret += "enemy-block\n" + matrixToString(user1->block);
        ret += "enemy-board\n" + matrixToString(user1->board);
        ret += "enemy-hold\n";
        ret += user1->holdBlock;
        ret += '\n';

        ret += "enemy-next\n";
        for (int i = 0; i < 3; i++) {
            ret += user1->nextBlock[i];
        }
        ret += '\n';

        ret += "my-score " + std::to_string(user2->score) + '\n';
        ret += "enemy-score " + std::to_string(user1->score) + '\n';
        m_server.send(hdl2, ret, websocketpp::frame::opcode::text);
    }
}

void GameQueue::pve(server &m_server, websocketpp::connection_hdl &hdl) {
    auto hdl1 = hdl;

    m_server.send(hdl1, "Matched", websocketpp::frame::opcode::text);

    User *user1 = new User;
    AI *ai = new AI;
    user1->hdl = hdl1;
    Pve *game = new Pve(user1, ai);
    pves[hdl1] = game;

    in_game.insert(hdl1);

    // 傳送初始狀態
    // user 1
    std::string ret = "my-block\n" + matrixToString(user1->block);
    ret += "my-board\n" + matrixToString(user1->board);

    ret += "my-hold\n";
    ret += user1->holdBlock;
    ret += '\n';

    ret += "my-next\n";
    for (int i = 0; i < 3; i++) {
        ret += user1->nextBlock[i];
    }
    ret += '\n';

    ret += "ai-block\n" + matrixToString(ai->block);
    ret += "ai-board\n" + matrixToString(ai->board);

    ret += "ai-next\n";
    for (int i = 0; i < 3; i++) {
        ret += ai->nextBlock[i];
    }
    ret += '\n';

    ret += "my-score " + std::to_string(user1->score) + '\n';
    ret += "ai-score " + std::to_string(ai->score) + '\n';
    m_server.send(hdl1, ret, websocketpp::frame::opcode::text);
}

bool GameQueue::isInGame(websocketpp::connection_hdl &hdl) {
    return in_game.find(hdl) != in_game.end();
}

bool GameQueue::isInPve(websocketpp::connection_hdl &hdl) {
    return in_game.find(hdl) != in_game.end();
}

Game *GameQueue::getGame(websocketpp::connection_hdl &hdl) {
    return games[hdl];
}

Pve *GameQueue::getPve(websocketpp::connection_hdl &hdl) {
    return pves[hdl];
}

void GameQueue::removeGame(websocketpp::connection_hdl &hdl) {
    in_game.erase(hdl);
    games.erase(hdl);
    // delete games[hdl];
}

void GameQueue::removePve(websocketpp::connection_hdl &hdl) {
    in_game.erase(hdl);
    pves.erase(hdl);
    // delete pves[hdl];
}