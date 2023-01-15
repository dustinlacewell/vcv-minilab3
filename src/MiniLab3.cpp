#include <rack.hpp>

#include "plugin.hpp"
#include "MiniLab3.hpp"
#include "menu/MenuSlider.cpp"
#include "ui/OutputPort.cpp"

// use the rack::dsp namespace for convenience
using namespace rack::dsp;

MiniLab3::MiniLab3() {
	config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
	configOutput(NOTE_OUTPUT, "10V Note");
	configOutput(GATE_OUTPUT, "10V Gate");
	configOutput(VELOCITY_OUTPUT, "10V Velocity");
	configOutput(BEND_OUTPUT, "1V Bend");
	configOutput(MOD_OUTPUT, "1V Mod");
	configOutput(KNOB1_OUTPUT, "1V Knob 1");
	configOutput(KNOB2_OUTPUT, "1V Knob 2");
	configOutput(KNOB3_OUTPUT, "1V Knob 3");
	configOutput(KNOB4_OUTPUT, "1V Knob 4");
	configOutput(KNOB5_OUTPUT, "1V Knob 5");
	configOutput(KNOB6_OUTPUT, "1V Knob 6");
	configOutput(KNOB7_OUTPUT, "1V Knob 7");
	configOutput(KNOB8_OUTPUT, "1V Knob 8");
	configOutput(SLIDER1_OUTPUT, "1V Slider 1");
	configOutput(SLIDER2_OUTPUT, "1V Slider 2");
	configOutput(SLIDER3_OUTPUT, "1V Slider 3");
	configOutput(SLIDER4_OUTPUT, "1V Slider 4");

}

json_t* MiniLab3::dataToJson() {
	json_t* rootJ = json_object();
	json_object_set_new(rootJ, "midiInput", midiInput.toJson());

	json_object_set_new(rootJ, "bendLimit", bendLimit->toJson());
	json_object_set_new(rootJ, "bend", bend->toJson());

	json_object_set_new(rootJ, "modLimit", modLimit->toJson());
	json_object_set_new(rootJ, "mod", mod->toJson());

	// knob strength: int
	json_object_set_new(rootJ, "knobStrength", json_integer(knobStrength));
	// knob voltage mode
	json_object_set_new(rootJ, "knobVoltageMode", json_integer(knobVoltageMode));
	// knob slew limit
	json_object_set_new(rootJ, "knobLimit", knobLimit->toJson());
	// 8 SlewVoltage->toJson objects from knobs as json array
	json_t* knobsJ = json_array();
	for (int i = 0; i < 8; i++) {
		json_array_append_new(knobsJ, knobs[i]->toJson());
	}
	json_object_set_new(rootJ, "knobs", knobsJ);

	// slider voltage mode
	json_object_set_new(rootJ, "sliderVoltageMode", json_integer(sliderVoltageMode));
	// slider slew limit
	json_object_set_new(rootJ, "sliderLimit", sliderLimit->toJson());
	// 4 SlewVoltage->toJson objects from sliders as json array
	json_t* slidersJ = json_array();
	for (int i = 0; i < 4; i++) {
		json_array_append_new(slidersJ, sliders[i]->toJson());
	}
	json_object_set_new(rootJ, "sliders", slidersJ);
	
	return rootJ;
}

void MiniLab3::dataFromJson(json_t* rootJ) {
	// midi input
	json_t* midiInputJ = json_object_get(rootJ, "midiInput");
	if (midiInputJ) {
		midiInput.fromJson(midiInputJ);
	}

	// bend slew limit
	json_t* bendLimitJ = json_object_get(rootJ, "bendLimit");
	if (bendLimitJ) {
		bendLimit->fromJson(bendLimitJ);
	}
	// bend voltage
	json_t* bendJ = json_object_get(rootJ, "bend");
	if (bendJ) {
		bend->fromJson(bendJ, bendLimit);
	}

	// mod slew limit
	json_t* modLimitJ = json_object_get(rootJ, "modLimit");
	if (modLimitJ) {
		modLimit->fromJson(modLimitJ);
	}
	// mod voltage
	json_t* modJ = json_object_get(rootJ, "mod");
	if (modJ) {
		mod->fromJson(modJ, modLimit);
	}

	// knob strength
	json_t* knobStrengthJ = json_object_get(rootJ, "knobStrength");
	if (knobStrengthJ) {
		knobStrength = json_integer_value(knobStrengthJ);
	}
	// knob voltage mode
	json_t* knobVoltageModeJ = json_object_get(rootJ, "knobVoltageMode");
	if (knobVoltageModeJ) {
		knobVoltageMode = (VoltageMode)json_integer_value(knobVoltageModeJ);
	}
	// knob slew limit
	json_t* knobLimitJ = json_object_get(rootJ, "knobLimit");
	if (knobLimitJ) {
		knobLimit->fromJson(knobLimitJ);
	}
	// knob voltages
	json_t* knobsJ = json_object_get(rootJ, "knobs");
	if (knobsJ) {
		for (int i = 0; i < 8; i++) {
			json_t* knobJ = json_array_get(knobsJ, i);
			if (knobJ) {
				knobs[i]->fromJson(knobJ, knobLimit);
			}
		}
	}


	// slider voltage mode
	json_t* sliderVoltageModeJ = json_object_get(rootJ, "sliderVoltageMode");
	if (sliderVoltageModeJ) {
		sliderVoltageMode = (VoltageMode)json_integer_value(sliderVoltageModeJ);
	}
	// slider slew limit
	json_t* sliderLimitJ = json_object_get(rootJ, "sliderLimit");
	if (sliderLimitJ) {
		sliderLimit->fromJson(sliderLimitJ);
	}
	// slider voltages
	json_t* slidersJ = json_object_get(rootJ, "sliders");
	if (slidersJ) {
		for (int i = 0; i < 4; i++) {
			json_t* sliderJ = json_array_get(slidersJ, i);
			if (sliderJ) {
				sliders[i]->fromJson(sliderJ, sliderLimit);
			}
		}
	}

}

void MiniLab3::process(const ProcessArgs& args) {

	midi::Message msg;
	while (midiInput.tryPop(&msg, args.frame)) {
		processMidi(msg);
	}

	outputs[BEND_OUTPUT].setVoltage(bend->getVoltage(args.sampleTime, args.sampleRate));
	outputs[MOD_OUTPUT].setVoltage(mod->getVoltage(args.sampleTime, args.sampleRate));
	for (int i = 0; i < 8; i++) {
		outputs[KNOB1_OUTPUT + i].setVoltage(knobs[i]->getVoltage(args.sampleTime, args.sampleRate));
	}
	for (int i = 0; i < 4; i++) {
		outputs[SLIDER1_OUTPUT + i].setVoltage(sliders[i]->getVoltage(args.sampleTime, args.sampleRate));
	}

}

bool MiniLab3::gateIsOpen() {
	for (int i = 0; i < 16; i++) {
		if (gates[i]) {
			return false;
		}
	}
	return true;
}

int MiniLab3::isGateKey(midi::Message& msg) {
	if (msg.getChannel() != 15) {
		return -1;
	}

	if (msg.getStatus() != 0x9 && msg.getStatus() != 0x8) {
		return -1;
	}

	int note = msg.getNote();
	switch (note) {
		case 36:
			return 0;
		case 48:
			return 1;
		case 60:
			return 2;
		case 72:
			return 3;
		case 84:
			return 4;
		case 96:
			return 5;
		case 108:
			return 6;
		case 120:
			return 7;
		case 38:
			return 8;
		case 50:
			return 9;
		case 62:
			return 10;
		case 74:
			return 11;
		case 86:
			return 12;
		case 98:
			return 13;
		case 110:
			return 14;
		case 122:
			return 15;
		default:
			return -1;	
	}
}

void MiniLab3::processMidi(midi::Message& msg) {
	int channel = msg.getChannel() + 1;
	int isPad = this->isGateKey(msg);

	if (isPad > -1) {
		switch (msg.getStatus()) {
			case 0x9: // note on
				gates[isPad] = true;
				break;
			case 0x8: // note off
				gates[isPad] = false;
				break;		
		}

		return;
	}

	bool gateIsOpen = this->gateIsOpen();

	switch (msg.getStatus()) {
		case 0x9: // note on
			processNoteOn(msg);				
			break;
		case 0x8: // note off
			if (isPad > -1) {
				gates[isPad] = false;
			} else {
				processNoteOff(msg);
			}
			break;
		case 0xe: // pitch bend
			if (gateIsOpen) {
				processPitchBend(msg);
			}			
			break;
		case 0xb: // control change
			processControlChange(msg);
			break;
		default:
			break;			
	}

	midiRelay.processMessage(msg);
}	

void MiniLab3::processNoteOn(midi::Message& msg) {
	int note = msg.getNote();
	float noteVoltage = note / 12.0;
	int velocity = msg.getValue();
	float velocityVoltage = velocity / 127.0 * 10.0;
	outputs[NOTE_OUTPUT].setVoltage(noteVoltage);
	outputs[VELOCITY_OUTPUT].setVoltage(velocityVoltage);
	outputs[GATE_OUTPUT].setVoltage(10.0);
}

void MiniLab3::processNoteOff(midi::Message& msg) {
	outputs[VELOCITY_OUTPUT].setVoltage(0);
	outputs[GATE_OUTPUT].setVoltage(0);
}

void MiniLab3::processPitchBend(midi::Message& msg) {
	bend->setTarget(((uint16_t)msg.getValue() << 7) | msg.getNote());
}

void MiniLab3::processKnob(int knob, int value) {
	if (value == 62) {
		value = -knobStrength;
	} else if (value == 66) {
		value = knobStrength;
	} else {
		value = 0;
	}
	SlewVoltage* slew = knobs[knob];
	float newTarget = slew->getTarget() + value;
	float clampedTarget = clamp(newTarget, 0.0f, 127.0f);
	slew->setTarget(clampedTarget);
}

void MiniLab3::processSlider(int slider, int value) {
	SlewVoltage* slew = sliders[slider];
	slew->setTarget(value);
}

void MiniLab3::processControlChange(midi::Message& msg) {
	int note = msg.getNote();
	bool gateIsOpen = this->gateIsOpen();
	switch (note) {
		case 1:
			if (gateIsOpen) {
				mod->setTarget(msg.getValue());
			}
			break;
		case 80:
			sliders[0]->setTarget(msg.getValue());
			break;
		case 81:
			sliders[1]->setTarget(msg.getValue());
			break;
		case 82:
			sliders[2]->setTarget(msg.getValue());
			break;
		case 83:
			sliders[3]->setTarget(msg.getValue());
			break;
		case 102:
		case 103:
		case 104:
		case 105:
		case 106:
		case 107:
		case 108:
		case 109:
			if (gateIsOpen) {
				processKnob(note - 102, msg.getValue());
			}
			break;
		default:
			break;
	}
}


struct MiniLab3Widget : ModuleWidget {
	MiniLab3Widget(MiniLab3* module) {
		setModule(module);
		setPanel(createPanel(asset::plugin(pluginInstance, "res/MiniLab3.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(9.382, 51.645)), module, MiniLab3::NOTE_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(30.48, 51.645)), module, MiniLab3::GATE_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(51.577, 51.645)), module, MiniLab3::VELOCITY_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(18.282, 66.756)), module, MiniLab3::BEND_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(41.677, 66.756)), module, MiniLab3::MOD_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(6.445, 84.75)), module, MiniLab3::KNOB1_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(22.119, 84.75)), module, MiniLab3::KNOB2_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(38.768, 84.75)), module, MiniLab3::KNOB3_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(56.03, 84.75)), module, MiniLab3::KNOB4_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(6.445, 97.364)), module, MiniLab3::KNOB5_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(22.119, 97.364)), module, MiniLab3::KNOB6_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(38.768, 97.364)), module, MiniLab3::KNOB7_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(55.428, 97.364)), module, MiniLab3::KNOB8_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(6.445, 115.842)), module, MiniLab3::SLIDER1_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(22.119, 115.842)), module, MiniLab3::SLIDER2_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(38.768, 115.842)), module, MiniLab3::SLIDER3_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(55.729, 115.842)), module, MiniLab3::SLIDER4_OUTPUT));

		MidiDisplay* display = createWidget<MidiDisplay>(mm2px(Vec(0.0, 13.f)));
		display->box.size = mm2px(Vec(60.960, 29.012));
		display->setMidiPort(module ? &module->midiInput : NULL);
		addChild(display);			
	}

	void appendContextMenu(Menu* menu) override {
		ModuleWidget::appendContextMenu(menu);

		// add menu slider for controlling bend slew
		MiniLab3* module = dynamic_cast<MiniLab3*>(this->module);
		assert(module);

		menu->addChild(new MenuSeparator());
		menu->addChild(createSubmenuItem("Bend", "",
			[=](Menu* menu) {
				menu->addChild(new MenuSlider(module->bendLimit));
				menu->addChild(createIndexPtrSubmenuItem("Voltage mode", {"0V:1V", "0V:10V", "-1V:1V", "-10V:10V"}, &module->bend->voltageMode));
			}
		));		
		
		menu->addChild(createSubmenuItem("Mod", "",
			[=](Menu* menu) {
				menu->addChild(new MenuSlider(module->modLimit));
				menu->addChild(createIndexPtrSubmenuItem("Voltage mode", {"0V:1V", "0V:10V", "-1V:1V", "-10V:10V"}, &module->mod->voltageMode));
			}
		));	
		
		menu->addChild(createSubmenuItem("Sliders", "",
			[=](Menu* menu) {
				menu->addChild(new MenuSlider(module->sliderLimit));
				menu->addChild(createSubmenuItem("Voltage mode", "",
					[=](Menu* menu) {
						menu->addChild(createCheckMenuItem("0V:1V", "", 
							[=]() { return module->sliderVoltageMode == VoltageMode::UNIPOLAR_1; }, 
							[=]() {  
								for (int i = 0; i < 4; i++) {
									module->sliders[i]->voltageMode = VoltageMode::UNIPOLAR_1;
								}
							}
						));
						menu->addChild(createCheckMenuItem("0V:10V", "", 
							[=]() { return module->sliderVoltageMode == VoltageMode::UNIPOLAR_10; }, 
							[=]() {  
								for (int i = 0; i < 4; i++) {
									module->sliders[i]->voltageMode = VoltageMode::UNIPOLAR_10;
								}
							}
						));
						menu->addChild(createCheckMenuItem("-1V:1V", "", 
							[=]() { return module->sliderVoltageMode == VoltageMode::BIPOLAR_1; }, 
							[=]() {  
								for (int i = 0; i < 4; i++) {
									module->sliders[i]->voltageMode = VoltageMode::BIPOLAR_1;
								}
							}
						));
						menu->addChild(createCheckMenuItem("-10V:10V", "", 
							[=]() { return module->sliderVoltageMode == VoltageMode::BIPOLAR_10; }, 
							[=]() {  
								for (int i = 0; i < 4; i++) {
									module->sliders[i]->voltageMode = VoltageMode::BIPOLAR_10;
								}
							}
						));
					}
				));
			}
		));	

		menu->addChild(createSubmenuItem("Knobs", "",
			[=](Menu* menu) {
				menu->addChild(new MenuSlider(module->knobLimit));
				menu->addChild(createSubmenuItem("Strength", "",
					[=](Menu* menu) {
						menu->addChild(createCheckMenuItem("1", "", 
							[=]() { return module->knobStrength == 1; }, 
							[=]() { module->knobStrength = 1; }
						));
						menu->addChild(createCheckMenuItem("2", "", 
							[=]() { return module->knobStrength == 2; }, 
							[=]() { module->knobStrength = 2; }
						));
						menu->addChild(createCheckMenuItem("4", "", 
							[=]() { return module->knobStrength == 4; }, 
							[=]() { module->knobStrength = 4; }
						));
						menu->addChild(createCheckMenuItem("8", "", 
							[=]() { return module->knobStrength == 8; }, 
							[=]() { module->knobStrength = 8; }
						));
					}
				));
				menu->addChild(createSubmenuItem("Voltage mode", "",
					[=](Menu* menu) {
						menu->addChild(createCheckMenuItem("0V:1V", "", 
							[=]() { return module->knobVoltageMode == VoltageMode::UNIPOLAR_1; }, 
							[=]() {  
								for (int i = 0; i < 8; i++) {
									module->knobs[i]->voltageMode = VoltageMode::UNIPOLAR_1;
								}
							}
						));
						menu->addChild(createCheckMenuItem("0V:10V", "", 
							[=]() { return module->knobVoltageMode == VoltageMode::UNIPOLAR_10; }, 
							[=]() {  
								for (int i = 0; i < 8; i++) {
									module->knobs[i]->voltageMode = VoltageMode::UNIPOLAR_10;
								}
							}
						));
						menu->addChild(createCheckMenuItem("-1V:1V", "", 
							[=]() { return module->knobVoltageMode == VoltageMode::BIPOLAR_1; }, 
							[=]() {  
								for (int i = 0; i < 8; i++) {
									module->knobs[i]->voltageMode = VoltageMode::BIPOLAR_1;
								}
							}
						));
						menu->addChild(createCheckMenuItem("-10V:10V", "", 
							[=]() { return module->knobVoltageMode == VoltageMode::BIPOLAR_10; }, 
							[=]() {  
								for (int i = 0; i < 8; i++) {
									module->knobs[i]->voltageMode = VoltageMode::BIPOLAR_10;
								}
							}
						));
					}
				));				
			}
		));
	}	
};


Model* modelMiniLab3 = createModel<MiniLab3, MiniLab3Widget>("MiniLab3");
