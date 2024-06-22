# 使用官方的 Ubuntu 基礎映像
FROM ubuntu:20.04

# 設置環境變量以避免在安裝軟件時出現交互提示
ENV DEBIAN_FRONTEND=noninteractive

# 安裝必要的軟件包
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    libboost-system-dev \
    libboost-thread-dev \
    libssl-dev \
    git \
    wget \
    python3 \
    python3-pip \
    unzip

# 安裝 websocketpp
RUN mkdir -p /usr/local/include/websocketpp && \
    wget https://github.com/zaphoyd/websocketpp/archive/refs/heads/master.zip -O /tmp/websocketpp.zip && \
    unzip /tmp/websocketpp.zip -d /tmp && \
    cp -r /tmp/websocketpp-master/websocketpp /usr/local/include/

# 設置工作目錄
WORKDIR /app

# 複製項目文件到工作目錄
COPY . /app

# 構建項目
RUN rm -rf build
RUN mkdir -p build && cd build && cmake .. && make

# 暴露應用程序運行的端口
EXPOSE 9024

# 設置容器啟動時運行的命令
CMD ["./build/my_websocket_project"]