#pragma once
#include <vector>
#include <websocketpp/server.hpp>

class User {
    public:
        std::vector<std::vector<char>> block;
        std::vector<std::vector<char>> board;
        websocketpp::connection_hdl hdl;
};

class Game {
    public:
        Game();
        Game(User *user1, User *user2);
        std::string operation(websocketpp::connection_hdl &hdl, std::string input);
        User *user1, *user2;
};

void printMatrix(const std::vector<std::vector<char>>& matrix);
bool canPlaceBlock(const std::vector<std::vector<char>>& board, const std::vector<std::vector<char>>& block, int startRow, int startCol);
void placeBlock(std::vector<std::vector<char>>& board, const std::vector<std::vector<char>>& block, int startRow, int startCol);
