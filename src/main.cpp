#include "websocket_server.hpp"

int main() {
    WebSocketServer server;
    server.run(9002);
    return 0;
}