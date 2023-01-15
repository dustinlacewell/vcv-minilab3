#pragma once

#include <rack.hpp>

#include "LedText.hpp"

struct TextLogWidget : Widget {
	LedDisplay* background;
	LedTextDisplay* ledDisplay;
	std::list<std::string> buffer;

	TextLogWidget();
	
	void step() override;

    void clear();

	void bufferToString();

	void push(std::string text);
};