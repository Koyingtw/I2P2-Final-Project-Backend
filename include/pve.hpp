#pragma once
#include <vector>
#include <websocketpp/server.hpp>
#include <websocketpp/config/asio_no_tls.hpp>
#include "user.hpp"
#include "ai.hpp"

typedef websocketpp::server<websocketpp::config::asio> server;

class Pve {
    public:
        Pve();
        Pve(User *u1, AI *ai);
        std::string operation(server &m_server, websocketpp::connection_hdl &hdl, std::string input);
        void hold(server &m_server, websocketpp::connection_hdl &hdl);
        User *user1;
        AI *ai;
};
