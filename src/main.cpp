#include "game_queue.hpp"
#include "websocket_server.hpp"
#include "constants.hpp"

int main() {
    GameQueue game_queue;
    WebSocketServer server(game_queue);
    server.run(PORT);
    return 0;
}