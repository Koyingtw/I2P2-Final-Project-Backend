#pragma once
#include <queue>
#include <set>
#include <map>
#include <websocketpp/server.hpp>
#include <websocketpp/config/asio_no_tls.hpp>
// #include "websocket_server.hpp"
#include "game.hpp"
#include "pve.hpp"
#include "operators.hpp"

typedef websocketpp::server<websocketpp::config::asio> server;


class GameQueue {
    private:
        std::queue<websocketpp::connection_hdl> user_queue;
        std::set<websocketpp::connection_hdl, connection_hdl_comparator> in_game;
        std::map<websocketpp::connection_hdl, Game*, connection_hdl_comparator> games;
        std::map<websocketpp::connection_hdl, Pve*, connection_hdl_comparator> pves;
    public:
        GameQueue();
        void push(server &m_server, websocketpp::connection_hdl &hdl);
        void pve(server &m_server, websocketpp::connection_hdl &hdl);
        bool isInGame(websocketpp::connection_hdl &hdl);
        bool isInPve(websocketpp::connection_hdl &hdl);
        Game *getGame(websocketpp::connection_hdl &hdl);
        Pve *getPve(websocketpp::connection_hdl &hdl);
        void removeGame(websocketpp::connection_hdl &hdl);
        void removePve(websocketpp::connection_hdl &hdl);
};