#include "FancyPanelBorder.hpp"

void FancyPanelBorder::draw(const DrawArgs& args) {
 	NVGcolor borderColor = nvgRGBAf(0.5, 0.5, 0.5, 0.5);

    // draw a line across the top   
	nvgBeginPath(args.vg);
    nvgMoveTo(args.vg, -0.5, 0.5);
    nvgLineTo(args.vg, box.size.x + 0.5, 0.5);
	nvgStrokeColor(args.vg, borderColor);
	nvgStrokeWidth(args.vg, 1.0);
	nvgStroke(args.vg);

    // draw a line across the bottom
    nvgBeginPath(args.vg);
    nvgMoveTo(args.vg, -0.5, box.size.y - 0.5);
    nvgLineTo(args.vg, box.size.x + 0.5, box.size.y - 0.5);
    nvgStrokeColor(args.vg, borderColor);
    nvgStrokeWidth(args.vg, 1.0);
    nvgStroke(args.vg);

    if (leftBorder) {
        //draw a line down the left side
        nvgBeginPath(args.vg);
        nvgMoveTo(args.vg, 0.5, 1);
        nvgLineTo(args.vg, 0.5, box.size.y - 1);
        nvgStrokeColor(args.vg, borderColor);
        nvgStrokeWidth(args.vg, 1.0);
        nvgStroke(args.vg);
    }

    if (rightBorder) {
        // draw a line down the right side
        nvgBeginPath(args.vg);
        nvgMoveTo(args.vg, box.size.x - 2.5, 1);
        nvgLineTo(args.vg, box.size.x - 2.5, box.size.y - 1);
        nvgStrokeColor(args.vg, borderColor);
        nvgStrokeWidth(args.vg, 1.0);
        nvgStroke(args.vg);
    }
}