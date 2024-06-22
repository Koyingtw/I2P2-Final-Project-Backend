#pragma once
#include <vector>
#include <deque>
#include <websocketpp/server.hpp>

class User {
    public:
        std::vector<std::vector<char>> block;
        std::vector<std::vector<char>> board;
        websocketpp::connection_hdl hdl;
        int score = 0;
        std::deque<char> nextBlock;
        std::deque<std::string> nextBlockStr;
        char holdBlock = 'X';
};
