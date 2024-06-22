#include "websocket_server.hpp"
#include "constants.hpp"
#include "game_queue.hpp"
#include "game.hpp"
#include "pve.hpp"
#include <iostream>
#include <string>

WebSocketServer::WebSocketServer(GameQueue &game_queue) {
    m_game_queue = game_queue;
    m_server.set_max_message_size(65536);
    m_server.init_asio();
    m_server.set_open_handler(bind(&WebSocketServer::on_open, this, std::placeholders::_1));
    m_server.set_close_handler(bind(&WebSocketServer::on_close, this, std::placeholders::_1));
    m_server.set_message_handler(bind(&WebSocketServer::on_message, this, std::placeholders::_1, std::placeholders::_2));
}

void WebSocketServer::run(uint16_t port) {
    m_server.listen(port);
    m_server.start_accept();
    m_server.run();
}

void WebSocketServer::on_open(websocketpp::connection_hdl hdl) {
    std::lock_guard<std::mutex> guard(m_connection_lock);
    m_connections.insert(hdl);
    std::cout << "Connection opened, total connections: " << m_connections.size() << std::endl;

    // 創建定時器並開始週期性地發送資料
    // send_periodic_data(hdl);
}

void WebSocketServer::on_close(websocketpp::connection_hdl hdl) {
    std::lock_guard<std::mutex> guard(m_connection_lock);
    m_connections.erase(hdl);
    std::cout << "Connection closed, total connections: " << m_connections.size() << std::endl;
}

void WebSocketServer::on_message(websocketpp::connection_hdl hdl, server::message_ptr msg) {
    std::string message = msg->get_payload();
    std::cout << "Received message: " << message << std::endl;

    if (message == "in queue") {
        m_server.send(hdl, "You are in queue", websocketpp::frame::opcode::text);
        m_game_queue.push(m_server, hdl);
    }
    else if (message[0] == '1' && m_game_queue.isInGame(hdl)) {
        Game *game = m_game_queue.getGame(hdl);
        auto hdl1 = (game->user1->hdl);
        auto hdl2 = (game->user2->hdl);
        // read the message
        std::string ret = game->operation(m_server, hdl, message);
        std::cout << "Return message: " << ret << std::endl;
        std::cout << "hdl: " << &hdl << std::endl;
        std::cout << "hdl1: " << &hdl1 << std::endl;
        std::cout << "hdl2: " << &hdl2 << std::endl;
        if (hdl1 == hdl)
            std::cout << "hdl1 and hdl are equal" << std::endl;
        if (hdl2 == hdl)
            std::cout << "hdl2 and hdl are equal" << std::endl;
        
        if (ret == "Game Over") {
            m_game_queue.removeGame(hdl1);
            m_game_queue.removeGame(hdl2);
        }
    }
    else if (message == "pve") {
        m_server.send(hdl, "PVE Mode", websocketpp::frame::opcode::text);
        m_game_queue.pve(m_server, hdl);
    }
    else if (message[0] == '2' && m_game_queue.isInPve(hdl)) {
        Pve *PVE = m_game_queue.getPve(hdl);
        std::cout << "PVE" << std::endl;
        std::string ret = PVE->operation(m_server, hdl, message);

        if (ret == "Game Over") {
            m_game_queue.removePve(hdl);
        }
    }
    else if (message[0] == '3' && m_game_queue.isInPve(hdl)) {
        Pve *PVE = m_game_queue.getPve(hdl);
        PVE->hold(m_server, hdl);
    }
    else
        m_server.send(hdl, "Invalid message", websocketpp::frame::opcode::text);

    m_server.send(hdl, "Message received", websocketpp::frame::opcode::text);
}

void WebSocketServer::send_periodic_data(websocketpp::connection_hdl hdl) {
    auto timer = std::make_shared<boost::asio::steady_timer>(m_server.get_io_service(), std::chrono::milliseconds(static_cast<int>(TICK_TIME * 1000)));
    timer->async_wait([this, hdl, timer](const boost::system::error_code& ec) {
        if (!ec) {
            std::lock_guard<std::mutex> guard(m_connection_lock);
            if (m_connections.find(hdl) != m_connections.end()) {
                m_server.send(hdl, "Periodic data", websocketpp::frame::opcode::text);
                send_periodic_data(hdl); // 重新設置定時器
            }
        }
    });
}