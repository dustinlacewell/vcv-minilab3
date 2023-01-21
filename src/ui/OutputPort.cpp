#include <rack.hpp>

#include "../G8Pad.hpp"
#include "OutputPort.hpp"

using namespace rack;

float angleSpreadDegrees = 40.0;

float minAngle = -M_PI * (180 - angleSpreadDegrees) / 180;
float maxAngle = M_PI * (180 - angleSpreadDegrees) / 180;

static constexpr float ringWidth_MM = 0.7f;
static constexpr float ringPad_MM = 0.5f;
static constexpr float ringWidth_PX = 1.5;

OutputPort::OutputPort() {
    this->param = nullptr;
    this->bwValue = nullptr;
    this->bwSlew = nullptr;
    this->updateDivider.setDivision(1);
    setSvg(Svg::load(asset::system("res/ComponentLibrary/PJ301M.svg")));
    this->sw->box.pos.x += 1.5;
    this->sw->box.pos.y += 1.5;
    this->contextMenus = std::vector<ModularContextMenuItem*>();
    setupWidgets();
}

void OutputPort::setParam(BaseParam* newParam) {
    param = newParam;
    lastValue = param->getNormalizedValue();
    lastMode = param->getVoltageMode();
    setupWidgets();
}

void OutputPort::appendContextMenu(Menu* menu) {
    menu->addChild(new MenuSeparator());
    for (auto contextMenu : this->contextMenus) {
        contextMenu->appendContextMenu(menu);
    }
};

NVGcolor interpolate(NVGcolor a, NVGcolor b, float t) {
    NVGcolor ret;
    ret.r = a.r + (b.r - a.r) * t;
    ret.g = a.g + (b.g - a.g) * t;
    ret.b = a.b + (b.b - a.b) * t;
    ret.a = a.a + (b.a - a.a) * t;
    return ret;
}

void OutputPort::drawValueRing(NVGcontext* vg) {
    if (param == nullptr) {
        return;
    }
    auto pv = lastValue = this->param->getNormalizedValue();

    float offset = 0.0f;

    float angle;
    angle = rack::math::rescale(pv, 0.0f, 1.0f, minAngle, maxAngle);
    float startAngle;
    bool isBipolar;
    switch (this->param->getVoltageMode()) {
        case VoltageMode::BIPOLAR_1:
        case VoltageMode::BIPOLAR_5:
        case VoltageMode::BIPOLAR_10:
            startAngle = 0.0f;
            isBipolar = true;
            break;
        case VoltageMode::UNIPOLAR_1:
        case VoltageMode::UNIPOLAR_5:
        case VoltageMode::UNIPOLAR_10:
            startAngle = minAngle;
            isBipolar = false;
            break;
    }

    float radius = rack::mm2px(ringWidth_MM * 2 + 7) * 0.5;
    nvgBeginPath(vg);
    nvgArc(
        vg,
        box.size.x * 0.5 - offset,
        box.size.y * 0.5 - offset,
        radius,
        startAngle - M_PI_2,
        angle - M_PI_2,
        startAngle < angle ? NVG_CW : NVG_CCW
    );
    nvgStrokeWidth(vg, 5);
    // red to green
    nvgStrokeColor(
        // d3b313 full alpha to no alpha

        vg,
        interpolate(
            nvgRGBA(0xd3, 0xb3, 0x13, 0xff),
            nvgRGBA(0xd3, 0xb3, 0x13, 0xff),
            isBipolar ? std::abs(pv - 0.5f) * 2.0f : pv
        )
    );
    nvgLineCap(vg, NVG_ROUND);
    nvgStroke(vg);

    nvgBeginPath(vg);
    nvgArc(
        vg,
        box.size.x * 0.5 - offset,
        box.size.y * 0.5 - offset,
        radius,
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
    if (param == nullptr) {
        return;
    }

    auto pv = this->param->getSlew();

    if (param->slewLimitQuantity->getValue() < 0.0001f) {
        pv = this->param->getNormalizedValue();
    }

    float offset = 0.0f;

    float angle;
    angle = rack::math::rescale(pv, 0.0f, 1.0f, minAngle, maxAngle);
    float startAngle;
    bool isBipolar;
    switch (this->param->getVoltageMode()) {
        case VoltageMode::BIPOLAR_1:
        case VoltageMode::BIPOLAR_5:
        case VoltageMode::BIPOLAR_10:
            startAngle = 0.0f;
            isBipolar = true;
            break;
        case VoltageMode::UNIPOLAR_1:
        case VoltageMode::UNIPOLAR_5:
        case VoltageMode::UNIPOLAR_10:
            startAngle = minAngle;
            isBipolar = false;
            break;
    }

    float radius = rack::mm2px(ringWidth_MM * 2 + 7) * 0.5;
    nvgBeginPath(vg);
    nvgArc(
        vg,
        box.size.x * 0.5 - offset,
        box.size.y * 0.5 - offset,
        radius,
        startAngle - M_PI_2,
        angle - M_PI_2,
        startAngle < angle ? NVG_CW : NVG_CCW
    );
    nvgStrokeWidth(vg, 2.5);
    nvgStrokeColor(
        vg,
        interpolate(
            nvgRGBA(0xd3, 0xb3, 0x13, 0xff),
            nvgRGBA(0xd3, 0xb3, 0x13, 0xff),
            isBipolar ? std::abs(pv - 0.5f) * 2.0f : pv
        )
    );
    nvgLineCap(vg, NVG_ROUND);
    nvgStroke(vg);

    auto w = box.size.y;
    auto h = box.size.x;

    auto ox = std::sin(angle) * radius + w / 2 - offset;
    auto oy = h - (std::cos(angle) * radius + h / 2) - offset;

    nvgBeginPath(vg);
    nvgEllipse(vg, ox, oy, 2.5, 2.5);
    nvgFillColor(vg, nvgRGBA(0xff, 0xff, 0xff, 0xff));
    nvgFill(vg);
}

void OutputPort::setupWidgets() {
    box.size = rack::mm2px(rack::Vec(12, 12));

    auto omm = rack::mm2px(ringWidth_MM * 2 + ringPad_MM);
    auto hmm = omm * 0.5;
    box.size.x += omm;
    box.size.y += omm;

    if (!bwValue) {
        // first time through reposition a few items to center the pointer and leave ring room
        auto ptrOffset = rack::mm2px(12 - 9) * 0.5;
        sw->box.pos.x += ptrOffset;
        sw->box.pos.y += ptrOffset;

        sw->box.pos.x += hmm;
        sw->box.pos.y += hmm;

        bwValue = new BufferedDrawFunctionWidgetOnLayer(
            rack::Vec(0, 0),
            box.size,
            [this](auto vg) { this->drawValueRing(vg); }
        );

        bwSlew = new BufferedDrawFunctionWidgetOnLayer(
            rack::Vec(0, 0),
            box.size,
            [this](auto vg) { this->drawSlewRing(vg); }
        );

        addChild(bwValue);
        addChild(bwSlew);
    }
    bwValue->dirty = true;
    bwSlew->dirty = true;
}

void OutputPort::step() {
    if (param == nullptr || bwValue == nullptr) {
        SvgPort::step();
        return;
    }

    //    float newValue = this->param->getNormalizedValue();
    //    VoltageMode newMode = this->param->getVoltageMode();
    if (this->updateDivider.process()) {
        bwValue->dirty = true;
        bwSlew->dirty = true;
    }
}