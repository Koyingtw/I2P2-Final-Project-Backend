#pragma once
#include <vector>
#include <websocketpp/server.hpp>

class User {
    public:
        std::vector<std::vector<char>> block;
        std::vector<std::vector<char>> board;
        websocketpp::connection_hdl hdl;
};
