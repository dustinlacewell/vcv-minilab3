#include "plugin.hpp"


Plugin* pluginInstance;

void init(Plugin* p) {
	pluginInstance = p;
	p->addModel(modelMiniLab3);
	p->addModel(modelG8Pad);
	p->addModel(modelSimpleSlew);
	p->addModel(modelMidiLog);
	p->addModel(modelMidiLogExt);
}
