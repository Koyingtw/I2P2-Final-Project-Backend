#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>

#include <iostream>
#include <string>

typedef websocketpp::client<websocketpp::config::asio_client> client;

class WebSocketClient {
public:
    WebSocketClient() {
        // 初始化 Asio
        m_client.init_asio();

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
            // 發送消息
            m_client.send(hdl, "Hello, server!", websocketpp::frame::opcode::text);
        });

        // 設置關閉處理器
        con->set_close_handler([](websocketpp::connection_hdl hdl) {
            std::cout << "Connection closed" << std::endl;
        });

        // 設置失敗處理器
        con->set_fail_handler([](websocketpp::connection_hdl hdl) {
            std::cout << "Connection failed" << std::endl;
        });

        // 設置連接
        m_client.connect(con);

        // 啟動 Asio 事件循環
        m_client.run();
    }

private:
    void on_message(websocketpp::connection_hdl hdl, client::message_ptr msg) {
        std::cout << "Received message: " << msg->get_payload() << std::endl;
        // 可以根據需要處理更多的消息
    }

    client m_client;
    websocketpp::connection_hdl m_hdl;
};

int main() {
    WebSocketClient ws_client;
    ws_client.run("ws://localhost:9002");
    return 0;
}