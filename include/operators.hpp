#pragma once

struct connection_hdl_comparator {
    bool operator()(const websocketpp::connection_hdl& lhs, const websocketpp::connection_hdl& rhs) const {
        return lhs.lock().get() < rhs.lock().get();
    }
};
