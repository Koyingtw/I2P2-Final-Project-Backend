#include "game_queue.hpp"
#include "websocket_server.hpp"

int main() {
    GameQueue game_queue;
    WebSocketServer server(game_queue);
    server.run(9002);
    return 0;
}