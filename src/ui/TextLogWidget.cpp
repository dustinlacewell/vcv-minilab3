#include <rack.hpp>

#include <ui/TextLogWidget.hpp>
#include <ui/LedText.hpp>


using namespace rack;

TextLogWidget::TextLogWidget() {
    ledDisplay = createWidget<LedTextDisplay>(Vec(0, 0));
    addChild(ledDisplay);
}

void TextLogWidget::step() {
    ledDisplay->box.size = box.size;
    Widget::step();
}

void TextLogWidget::clear() {
    buffer.clear();
    bufferToString();
}

void TextLogWidget::bufferToString() {
    std::string s = "";
    for (std::string m : buffer) {
        s += m + "\n";
    }
    ledDisplay->text = s;
}

void TextLogWidget::push(std::string text) {
    buffer.push_back(text);

    if (buffer.size() > 26) {
        buffer.pop_front();
    }

    bufferToString();
}

