#include "operators.hpp"

bool operator==(const std::weak_ptr<void>& lhs, const std::weak_ptr<void>& rhs) {
    return !lhs.owner_before(rhs) && !rhs.owner_before(lhs);
}