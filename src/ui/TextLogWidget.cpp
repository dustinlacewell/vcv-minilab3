#include "../plugin.hpp"

#include "LedText.cpp"

struct TextLogWidget : Widget {
	LedDisplay* background;
	LedTextDisplay* ledDisplay;

	std::list<std::string> buffer;

	TextLogWidget() {
		ledDisplay = createWidget<LedTextDisplay>(Vec(0, 0));
		addChild(ledDisplay);
	}
	
	void step() override {
		ledDisplay->box.size = box.size;
		Widget::step();
	}

    void clear() {
        buffer.clear();
        bufferToString();
    }

	void bufferToString() {
		std::string s = "";
		for (std::string m : buffer) {
			s += m + "\n";
		}
		ledDisplay->text = s;
	}

	void push(std::string text) {
		buffer.push_back(text);

		if (buffer.size() > 26) {
			buffer.pop_front();
		}

		bufferToString();
	}
};


