#pragma once

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <set>

typedef websocketpp::server<websocketpp::config::asio> server;

class WebSocketServer {
public:
    WebSocketServer();
    void run(uint16_t port);

private:
    void on_open(websocketpp::connection_hdl hdl);
    void on_close(websocketpp::connection_hdl hdl);
    void on_message(websocketpp::connection_hdl hdl, server::message_ptr msg);

    server m_server;
    std::set<websocketpp::connection_hdl, std::owner_less<websocketpp::connection_hdl>> m_connections;
    std::mutex m_connection_lock;
};