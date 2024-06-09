cmake_minimum_required(VERSION 3.10)
project(my_websocket_project)

set(CMAKE_CXX_STANDARD 11)

# 包含 websocket++ 的頭文件路徑
include_directories(${CMAKE_SOURCE_DIR}/include)

# 添加 Boost 庫的路徑
find_package(Boost REQUIRED system thread)

# 添加可執行文件
add_executable(my_websocket_project
    src/main.cpp
    src/websocket_server.cpp
)

# 連結必要的庫 (Boost 庫)
target_link_libraries(my_websocket_project Boost::system Boost::thread)