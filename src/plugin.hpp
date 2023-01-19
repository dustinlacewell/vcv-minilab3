#pragma once
#include <rack.hpp>


using namespace rack;

// Declare the Plugin, defined in plugin.cpp
extern Plugin *pluginInstance;

// Declare each Model, defined in each module source file
extern Model *modelMiniLab3;
extern Model *modelG8Pad;
extern Model *modelSimpleSlew;
extern Model *modelMidiLog;
extern Model *modelMidiLogExt;
