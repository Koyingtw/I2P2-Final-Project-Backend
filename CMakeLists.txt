cmake_minimum_required(VERSION 3.10)
project(my_websocket_project)

set(CMAKE_CXX_STANDARD 11)

# set flag with -g
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fsanitize=address")

# 包含 websocket++ 的頭文件路徑
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(/opt/homebrew/Cellar/websocketpp/0.8.2/include)
include_directories(/opt/homebrew/Cellar/boost/1.85.0/include)

# 添加 Boost 庫的路徑
find_package(Boost REQUIRED system thread)

# 添加可執行文件
add_executable(my_websocket_project
    src/main.cpp
    src/websocket_server.cpp
    src/game_queue.cpp
    src/game.cpp
    src/operators.cpp
    src/ai.cpp
    src/pve.cpp
    src/matrix.cpp
)

target_link_libraries(my_websocket_project Boost::system Boost::thread pthread)

add_executable(websocket_test
    test/test_websocket.cpp
)

# 連結必要的庫 (Boost 庫)
target_link_libraries(websocket_test Boost::system Boost::thread pthread)