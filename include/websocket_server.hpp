#pragma once

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <boost/asio/steady_timer.hpp>
#include <set>
#include <mutex>
#include <iostream>
#include <chrono>
#include <memory>
#include "game_queue.hpp"
#include "operators.hpp"


typedef websocketpp::server<websocketpp::config::asio> server;


class WebSocketServer {
public:
    WebSocketServer(GameQueue &game_queue);
    void run(uint16_t port);
    server m_server;

private:
    void on_open(websocketpp::connection_hdl hdl);
    void on_close(websocketpp::connection_hdl hdl);
    void on_message(websocketpp::connection_hdl hdl, server::message_ptr msg);
    void send_periodic_data(websocketpp::connection_hdl hdl);
    

    GameQueue m_game_queue;
    std::set<websocketpp::connection_hdl, std::owner_less<websocketpp::connection_hdl>> 
        m_connections;
    std::mutex m_connection_lock;
};