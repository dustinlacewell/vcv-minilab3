#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif

#include <rack.hpp>
#include <math.h>
#include "MiniPad.hpp"
#include "OutputPort.hpp"

using namespace rack;
using namespace rack::math;

// Configuration constants
static constexpr float ANGLE_SPREAD_DEGREES = 40.0f;
static constexpr float MIN_ANGLE = -M_PI * (180 - ANGLE_SPREAD_DEGREES) / 180;
static constexpr float MAX_ANGLE = M_PI * (180 - ANGLE_SPREAD_DEGREES) / 180;
static constexpr float RING_WIDTH_PX = 2.0669291338582676f;
static constexpr float RING_PAD_PX = 1.4763779527559056f;
static constexpr float RING_RADIUS_PX = 12.401574803149607f;
static constexpr float PORT_BASE_SIZE = 43.0f;

// Static offsets
static constexpr float ring_offset_x = -5.f;
static constexpr float ring_offset_y = 0.f;

// Geometric transformation helpers
inline float toDisplayAngle(float angle) {
    return -(angle - M_PI_2);
}

inline Vec polarToCartesian(float angle, float radius) {
    float displayAngle = toDisplayAngle(angle);
    return Vec(
        std::cos(displayAngle) * radius,
        -std::sin(displayAngle) * radius
    );
}

inline Vec offsetToCenter(Vec pos, Vec center, Vec ringOffset) {
    return pos + center - ringOffset;
}

// Value mapping helpers
inline float normalizedToAngle(float normalizedValue) {
    return rack::math::rescale(normalizedValue, 0.0f, 1.0f, MIN_ANGLE, MAX_ANGLE);
}

inline float getStartAngle(VoltageMode mode) {
    return mode == VoltageMode::BIPOLAR_1 || 
           mode == VoltageMode::BIPOLAR_5 || 
           mode == VoltageMode::BIPOLAR_10 ? 0.0f : MIN_ANGLE;
}

inline bool isBipolarMode(VoltageMode mode) {
    return mode == VoltageMode::BIPOLAR_1 || 
           mode == VoltageMode::BIPOLAR_5 || 
           mode == VoltageMode::BIPOLAR_10;
}

// Color helpers
inline NVGcolor interpolateColors(NVGcolor a, NVGcolor b, float t) {
    return nvgRGBAf(
        a.r + (b.r - a.r) * t,
        a.g + (b.g - a.g) * t,
        a.b + (b.b - a.b) * t,
        a.a + (b.a - a.a) * t
    );
}

inline NVGcolor getRingColor(float normalizedValue, bool isBipolar) {
    auto baseColor = nvgRGBA(0xd3, 0xb3, 0x13, 0xff);
    float t = isBipolar ? std::abs(normalizedValue - 0.5f) * 2.0f : normalizedValue;
    return interpolateColors(baseColor, baseColor, t);
}

OutputPort::OutputPort() {
    param = nullptr;
    bwValue = nullptr;
    bwSlew = nullptr;
    updateDivider.setDivision(1);
    setSvg(Svg::load(asset::system("res/ComponentLibrary/PJ301M.svg")));
    contextMenus = std::vector<ModularContextMenuItem*>();
    
    auto originalY = box.size.y;
    box.size.y *= 1.5;
    sw->box.pos.y = box.size.y / 2 - originalY / 2;
    setupWidgets();
}

void OutputPort::drawValueRing(NVGcontext* vg) {
    if (!param) return;

    float normalizedValue = lastValue = param->getNormalizedValue();
    float angle = normalizedToAngle(normalizedValue);
    float startAngle = getStartAngle(param->getVoltageMode());
    bool isBipolar = isBipolarMode(param->getVoltageMode());
    Vec center = Vec(box.size.x * 0.5, box.size.y * 0.5);
    Vec ringOffset = Vec(ring_offset_x, ring_offset_y);

    // Draw main ring
    nvgBeginPath(vg);
    nvgArc(
        vg,
        center.x - ringOffset.x,
        center.y - ringOffset.y,
        RING_RADIUS_PX,
        startAngle - M_PI_2,
        angle - M_PI_2,
        startAngle < angle ? NVG_CW : NVG_CCW
    );
    nvgStrokeWidth(vg, 5);
    nvgStrokeColor(vg, getRingColor(normalizedValue, isBipolar));
    nvgLineCap(vg, NVG_ROUND);
    nvgStroke(vg);

    // Draw outline
    nvgBeginPath(vg);
    nvgArc(
        vg,
        center.x - ringOffset.x,
        center.y - ringOffset.y,
        RING_RADIUS_PX,
        startAngle - M_PI_2,
        angle - M_PI_2,
        startAngle < angle ? NVG_CW : NVG_CCW
    );
    nvgStrokeWidth(vg, 2);
    nvgStrokeColor(vg, nvgRGBA(0x00, 0x00, 0x00, 0xff));
    nvgLineCap(vg, NVG_ROUND);
    nvgStroke(vg);
}

void OutputPort::drawSlewRing(NVGcontext* vg) {
    if (!param) return;

    float normalizedValue = param->getSlew();
    if (param->slewLimitQuantity->getValue() < 0.0001f) {
        normalizedValue = param->getNormalizedValue();
    }

    float angle = normalizedToAngle(normalizedValue);
    float startAngle = getStartAngle(param->getVoltageMode());
    bool isBipolar = isBipolarMode(param->getVoltageMode());
    Vec center = Vec(box.size.x * 0.5, box.size.y * 0.5);
    Vec ringOffset = Vec(ring_offset_x, ring_offset_y);

    // Draw slew ring
    nvgBeginPath(vg);
    nvgArc(
        vg,
        center.x - ringOffset.x,
        center.y - ringOffset.y,
        RING_RADIUS_PX,
        startAngle - M_PI_2,
        angle - M_PI_2,
        startAngle < angle ? NVG_CW : NVG_CCW
    );
    nvgStrokeWidth(vg, 2.5);
    nvgStrokeColor(vg, getRingColor(normalizedValue, isBipolar));
    nvgLineCap(vg, NVG_ROUND);
    nvgStroke(vg);

    // Draw indicator dot
    Vec dotPos = polarToCartesian(angle, RING_RADIUS_PX);
    dotPos = offsetToCenter(dotPos, center, ringOffset);

    nvgBeginPath(vg);
    nvgEllipse(vg, dotPos.x, dotPos.y, 2.5, 2.5);
    nvgFillColor(vg, nvgRGBA(0xff, 0xff, 0xff, 0xff));
    nvgFill(vg);
}

void OutputPort::setParam(BaseParam* newParam) {
    param = newParam;
    lastValue = param->getNormalizedValue();
    lastMode = param->getVoltageMode();
    setupWidgets();
}

void OutputPort::setValue(float v) {
    if (param) {
        param->setValue(v);
    }
}

void OutputPort::appendContextMenu(Menu* menu) {
    menu->addChild(new MenuSeparator());
    for (auto contextMenu : contextMenus) {
        contextMenu->appendContextMenu(menu);
    }
}

void OutputPort::setupWidgets() {
    if (!bwValue) {
        bwValue = new BufferedDrawFunctionWidgetOnLayer(
            Vec(-5, 0),
            box.size.mult(3),
            [this](auto vg) { this->drawValueRing(vg); }
        );

        bwSlew = new BufferedDrawFunctionWidgetOnLayer(
            Vec(-5, 0),
            box.size.mult(3),
            [this](auto vg) { this->drawSlewRing(vg); }
        );

        addChild(bwValue);
        addChild(bwSlew);
    }
    bwValue->dirty = true;
    bwSlew->dirty = true;
}

void OutputPort::step() {
    if (!param || !bwValue) {
        SvgPort::step();
        return;
    }

    if (updateDivider.process()) {
        bwValue->dirty = true;
        bwSlew->dirty = true;
    }
}

void OutputPort::onDoubleClick(const event::DoubleClick& e) {
    if (!param) return;

    switch (param->getVoltageMode()) {
        case VoltageMode::BIPOLAR_1:
        case VoltageMode::BIPOLAR_5:
        case VoltageMode::BIPOLAR_10:
            param->setValue(64);
            break;
        case VoltageMode::UNIPOLAR_1:
        case VoltageMode::UNIPOLAR_5:
        case VoltageMode::UNIPOLAR_10:
            param->setValue(0);
            break;
    }
}