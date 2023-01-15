#include "../plugin.hpp"


struct MenuSlider : ui::Slider {
	MenuSlider(Quantity* q) {
		box.size.x = 220.0f;
		quantity = q;
	}
};