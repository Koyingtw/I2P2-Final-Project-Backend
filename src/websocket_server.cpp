#include "websocket_server.hpp"
#include "constants.hpp"
#include <iostream>

WebSocketServer::WebSocketServer() {
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
    send_periodic_data(hdl);
}

void WebSocketServer::on_close(websocketpp::connection_hdl hdl) {
    std::lock_guard<std::mutex> guard(m_connection_lock);
    m_connections.erase(hdl);
    std::cout << "Connection closed, total connections: " << m_connections.size() << std::endl;
}

void WebSocketServer::on_message(websocketpp::connection_hdl hdl, server::message_ptr msg) {
    std::cout << "Received message: " << msg->get_payload() << std::endl;
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