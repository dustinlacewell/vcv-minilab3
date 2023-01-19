#pragma once

#include <rack.hpp>

#include <G8Pad.hpp>


json_t *G8Pad::dataToJson() {
    json_t *rootJ = json_object();
    json_object_set_new(rootJ, "midiInput", midiInput.toJson());
    json_object_set_new(rootJ, "padMidi", padMidi->toJson());
    json_object_set_new(rootJ, "bend", bend->toJson());
    json_object_set_new(rootJ, "mod", mod->toJson());
    json_object_set_new(rootJ, "touch", touch->toJson());

    json_t *knobsJ = json_array();
    for (int i = 0; i < 8; i++) {
        json_t *knobJ = knobs[i]->toJson();
        json_array_append_new(knobsJ, knobJ);
    }

    return rootJ;
}

void G8Pad::dataFromJson(json_t *rootJ) {
    json_t *bendJ = json_object_get(rootJ, "bend");
    if (bendJ) {
        bend->fromJson(bendJ);
    }

    json_t *modJ = json_object_get(rootJ, "mod");
    if (modJ) {
        mod->fromJson(modJ);
    }

    json_t *touchJ = json_object_get(rootJ, "touch");
    if (touchJ) {
        touch->fromJson(touchJ);
    }

    json_t *knobsJ = json_object_get(rootJ, "knobs");
    if (knobsJ) {
        for (int i = 0; i < 8; i++) {
            json_t *knobJ = json_array_get(knobsJ, i);
            if (knobJ) {
                knobs[i]->fromJson(knobJ);
            }
        }
    }
}