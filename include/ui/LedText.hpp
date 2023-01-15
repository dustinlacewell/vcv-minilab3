#pragma once

#include <rack.hpp>

using namespace rack;


struct LedTextDisplay : OpaqueWidget {
	std::string text;
	float fontSize;
	math::Vec textOffset;
	NVGcolor color;
	NVGcolor bgColor;

	LedTextDisplay();
	void drawLayer(const DrawArgs& args, int layer) override;
};