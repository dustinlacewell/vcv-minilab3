#pragma once

#include <rack.hpp>

using namespace rack;

struct FancyPanelBorder : widget::TransparentWidget {
    NVGcolor bgFill = nvgRGBAf(1.0, 0.0, 0.0, 1.0);
    bool leftBorder = true;
    bool rightBorder = true;
	void draw(const DrawArgs& args) override;
};