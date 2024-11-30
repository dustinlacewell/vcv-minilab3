#include "plugin.hpp"


rack::plugin::Plugin* pluginInstance;


void init(rack::plugin::Plugin* p) {
	pluginInstance = p;
    p->addModel(modelMiniLab3);
    p->addModel(modelG8Pad);
    p->addModel(modelMiniLog);
}
