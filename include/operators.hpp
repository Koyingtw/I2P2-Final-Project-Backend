#pragma once
#include <websocketpp/server.hpp>
#include <websocketpp/config/asio_no_tls.hpp>

struct connection_hdl_comparator {
    bool operator()(const websocketpp::connection_hdl& lhs, const websocketpp::connection_hdl& rhs) const {
        return lhs.lock().get() < rhs.lock().get();
    }
};

bool operator ==(const websocketpp::connection_hdl& lhs, const websocketpp::connection_hdl& rhs);