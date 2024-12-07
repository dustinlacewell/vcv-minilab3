#pragma once

inline float getBrightness(bool ready, bool active) {
    return ready ? (active ? 1.0f : 0.15f) : 0.0f;
}