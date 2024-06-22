#include "game_queue.hpp"
#include "game.hpp"

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
        User user1;
        user1.hdl = &hdl1;
        User user2;
        user2.hdl = &hdl2;
        Game game(user1, user2);
        games[hdl1] = game;
        games[hdl2] = game;

        in_game.insert(hdl1);
        in_game.insert(hdl2);
    }
}

bool GameQueue::isInGame(websocketpp::connection_hdl hdl) {
    return in_game.find(hdl) != in_game.end();
}

Game GameQueue::getGame(websocketpp::connection_hdl hdl) {
    return games[hdl];
}