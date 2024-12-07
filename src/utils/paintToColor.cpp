#include "paintToColor.hpp"


NVGcolor paintToColor(uint32_t color) {
    // return {
    //     static_cast<float>((color >> 24) & 0xFF) / 255.0f,
    //     static_cast<float>((color >> 16) & 0xFF) / 255.0f,
    //     static_cast<float>((color >> 8) & 0xFF) / 255.0f,
    //     static_cast<float>(color & 0xFF) / 255.0f
    // };
    // just return red for now
    return nvgRGBAf(1.0, 0.0, 0.0, 1.0);
}