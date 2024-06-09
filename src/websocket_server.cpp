#include "websocket_server.hpp"
#include <iostream>

WebSocketServer::WebSocketServer() { // WebSocketServer constructor
    // Initialize the server, 
    // asio 是一個 C++ 的網路程式庫，提供了一個非同步的 I/O 模型，可以用來實作網路應用程式。
    m_server.init_asio(); 

    // 用來綁定連線開始、連線結束、接收訊息三種事件的事件處理器，第一個參數代表遇到那個事件時要執行的函式
    // std::placeholders::_1 代表這個綁定的函數被呼叫的時候，第一個參數會被傳入函數中（如果寫 2 就是第兩個）
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