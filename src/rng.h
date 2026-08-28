#pragma once

#include <cstdint>

namespace avatar_rng {

// xorshift32 has a specified sequence and does not depend on the C runtime.
inline uint32_t next(uint32_t& state) {
    if (state == 0) state = 0x6D2B79F5u;
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    return state;
}

inline int bounded(uint32_t& state, int count) {
    return count > 0 ? static_cast<int>(next(state) % static_cast<uint32_t>(count)) : 0;
}

} // namespace avatar_rng
