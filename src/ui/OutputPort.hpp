#pragma once

#include <rack.hpp>

#include "../MiniLab.hpp"
#include "../menu/ModularContextMenuItem.hpp"

using namespace rack;

struct BufferedDrawFunctionWidget : virtual rack::FramebufferWidget {
    typedef std::function<void(NVGcontext*)> drawfn_t;
    drawfn_t drawf;

    struct InternalBDW : rack::TransparentWidget {
        drawfn_t drawf;
        InternalBDW(rack::Rect box_, drawfn_t draw_) : drawf(draw_) {
            box = box_;
        }

        void draw(const DrawArgs& args) override { drawf(args.vg); }
    };

    InternalBDW* kid = nullptr;
    BufferedDrawFunctionWidget(rack::Vec pos, rack::Vec sz, drawfn_t draw_)
        : drawf(draw_) {
        box.pos = pos;
        box.size = sz;
        auto kidBox = rack::Rect(rack::Vec(0, 0), box.size);
        kid = new InternalBDW(kidBox, drawf);
        addChild(kid);
    }
};

struct BufferedDrawFunctionWidgetOnLayer : BufferedDrawFunctionWidget {
    int layer{1};
    BufferedDrawFunctionWidgetOnLayer(
        rack::Vec pos,
        rack::Vec sz,
        drawfn_t draw_,
        int ly = 1
    )
        : BufferedDrawFunctionWidget(pos, sz, draw_), layer(ly) {}

    void draw(const DrawArgs& args) override { return; }
    void drawLayer(const DrawArgs& args, int dl) override {
        if (dl == layer) {
            BufferedDrawFunctionWidget::draw(args);
        }
    }
};

struct OutputPort : SvgPort {
    // vector of ModularContextMenuItems
    BaseParam* param;
    float lastValue = 0.0f;
    VoltageMode lastMode = VoltageMode::UNIPOLAR_1;
    std::vector<ModularContextMenuItem*> contextMenus;
    dsp::ClockDivider updateDivider;

    BufferedDrawFunctionWidget* bwValue{nullptr};
    BufferedDrawFunctionWidget* bwSlew{nullptr};

    OutputPort();
    void setParam(BaseParam* param);
    void appendContextMenu(Menu* menu) override;
    void step() override;
    void drawValueRing(NVGcontext* vg);
    void drawSlewRing(NVGcontext* vg);
    //    void drawValueCap(NVGcontext* vg);
    void setupWidgets();
    void setValue(float v);

    void onDoubleClick(const event::DoubleClick& e) override;
};