#include "plugin.hpp"

rack::plugin::Plugin* pluginInstance;

void init(rack::plugin::Plugin* p) {
    pluginInstance = p;
    p->addModel(modelMiniLab);
    p->addModel(modelMiniPad);
    p->addModel(modelMiniLog);
}
