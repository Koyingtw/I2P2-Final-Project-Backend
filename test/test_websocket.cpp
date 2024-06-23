#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>

#include <iostream>
#include <string>
#include <thread>
#include <atomic>
#include "constants.hpp"

typedef websocketpp::client<websocketpp::config::asio_client> client;

class WebSocketClient {
public:
    WebSocketClient() : running(true) {
        // 初始化 Asio
        m_client.init_asio();
        m_client.set_max_message_size(65536);

        // 設置消息處理器
        m_client.set_message_handler(bind(&WebSocketClient::on_message, this, std::placeholders::_1, std::placeholders::_2));
    }

    void run(const std::string& uri) {
        websocketpp::lib::error_code ec;

        // 創建連接
        client::connection_ptr con = m_client.get_connection(uri, ec);
        if (ec) {
            std::cout << "Could not create connection because: " << ec.message() << std::endl;
            return;
        }

        // 設置開放處理器
        con->set_open_handler([this](websocketpp::connection_hdl hdl) {
            m_hdl = hdl;
            std::cout << "Connection opened" << std::endl;
            // 發送初始消息
            send_message("Hello, server!");
            send_message("pvp");
            // send_message("pve");
        });

        // 設置關閉處理器
        con->set_close_handler([this](websocketpp::connection_hdl hdl) {
            std::cout << "Connection closed" << std::endl;
            running = false;
        });

        // 設置失敗處理器
        con->set_fail_handler([this](websocketpp::connection_hdl hdl) {
            std::cout << "Connection failed" << std::endl;
            running = false;
        });

        // 設置連接
        m_client.connect(con);

        // 在另一個線程中運行 Asio 事件循環
        std::thread asio_thread([this]() {
            m_client.run();
        });

        // 在主線程中處理用戶輸入
        handle_user_input();

        // 等待 Asio 事件循環結束
        asio_thread.join();
    }

private:
    void send_message(const std::string& message) {
        try {
            websocketpp::lib::error_code ec;
            m_client.send(m_hdl, message, websocketpp::frame::opcode::text, ec);
            if (ec) {
                std::cerr << "Error sending message: " << ec.message() << std::endl;
            }
        } catch (const websocketpp::exception& e) {
            std::cerr << "Exception caught while sending message: " << e.what() << std::endl;
        }
    }

    void handle_user_input() {
        while (running) {
            int status;
            std::cin >> status;
            if (status == 2 || status == 1) {
                std::string user_block = std::to_string(status) + "XXXXXXXXXXXXXXXXXXXXXXXTXXXXXXXXTTTXXXXX\n";
                std::cout << "User block: " << user_block << std::endl;
                send_message(user_block);
            }
            else if (status == 3) {
                send_message("3");
            }
        }
    }

    void on_message(websocketpp::connection_hdl hdl, client::message_ptr msg) {
        std::string message = msg->get_payload();
        std::cout << "Received message: " << message << std::endl;
    }

    client m_client;
    websocketpp::connection_hdl m_hdl;
    std::atomic<bool> running;
};

int main() {
    WebSocketClient ws_client;
    std::string uri = "ws://localhost:" + std::to_string(PORT);
    ws_client.run(uri);
    return 0;
}