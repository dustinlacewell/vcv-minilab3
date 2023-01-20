#include <rack.hpp>

#include "LedText.hpp"

using namespace rack;

LedTextDisplay::LedTextDisplay() {
    fontSize = 12.f;
    color = nvgRGB(0xff, 0xd7, 0x14);
    bgColor = color::BLACK;
    textOffset = math::Vec(5, 4);
}

void LedTextDisplay::drawLayer(const DrawArgs& args, int layer) {
    nvgScissor(args.vg, RECT_ARGS(args.clipBox));
    if (layer == 1) {
        if (bgColor.a > 0.0) {
            nvgBeginPath(args.vg);
            nvgRect(args.vg, 0, 0, box.size.x, box.size.y);
            nvgFillColor(args.vg, bgColor);
            nvgFill(args.vg);
        }

        std::shared_ptr<Font> font = APP->window->loadFont(
            asset::system("res/fonts/ShareTechMono-Regular.ttf")
        );
        nvgFillColor(args.vg, color);
        nvgFontFaceId(args.vg, font->handle);
        nvgTextLetterSpacing(args.vg, 0.0);
        nvgFontSize(args.vg, 12);
        nvgTextBox(
            args.vg, textOffset.x, textOffset.y + fontSize,
            box.size.x - 2 * textOffset.x, text.c_str(), nullptr
        );
    }
    nvgResetScissor(args.vg);
}