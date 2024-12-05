#include <rack.hpp>

using namespace rack;

static const float MY_SVG_DPI = 75.f;
static const float MY_MM_PER_IN = 25.4f;

inline math::Vec mmm2px(math::Vec mm) {
    auto result = mm.mult(MY_SVG_DPI / MY_MM_PER_IN);
    return result;
}

inline math::Vec px2mm(math::Vec px) {
    return px.mult(MY_MM_PER_IN / MY_SVG_DPI);
}
