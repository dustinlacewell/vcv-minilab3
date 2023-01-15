#include "plugin.hpp"

#include "MiniLab3.hpp"
#include "ui/LedText.cpp"
#include "slew/slew.hpp"
#include "utils/Relay.cpp"
#include "utils/RelayCallback.cpp"
#include "menu/MenuSlider.cpp"


using namespace rack::dsp;


struct G8Pad : Module {
	enum ParamId {
		PARAMS_LEN
	};
	enum InputId {
		INPUTS_LEN
	};
	enum OutputId {
		GATE_OUTPUT,
		VELOCITY_OUTPUT,
		BEND_OUTPUT,
		TOUCH_OUTPUT,
		MOD_OUTPUT,
		KNOB1_OUTPUT,
		KNOB2_OUTPUT,
		KNOB3_OUTPUT,
		KNOB4_OUTPUT,
		KNOB5_OUTPUT,
		KNOB6_OUTPUT,
		KNOB7_OUTPUT,
		KNOBS8_OUTPUT,
		OUTPUTS_LEN
	};
	enum LightId {
		GATE_LIGHT,
		LIGHTS_LEN
	};

	int padId = -1;
	bool gateOpen = false;

	midi::InputQueue midiInput;
	dsp::ClockDivider divider;

	SlewLimitQuantity* bendLimit = new SlewLimitQuantity("Bend Slew", 0, 10);
	SlewVoltage* bend = new SlewVoltage(bendLimit, VoltageMode::BIPOLAR_1, 0, 16383);

	SlewLimitQuantity* modLimit = new SlewLimitQuantity("Mod Slew", 0, 10);
	SlewVoltage* mod = new SlewVoltage(modLimit);	

	SlewLimitQuantity* touchLimit = new SlewLimitQuantity("Touch Slew", 0, 10);
	SlewVoltage* touch = new SlewVoltage(touchLimit, VoltageMode::BIPOLAR_10, 0, 127);	

	int knobStrength = 2;	
	VoltageMode knobVoltageMode = VoltageMode::BIPOLAR_1;
	SlewLimitQuantity* knobLimit = new SlewLimitQuantity("Knob Slew", 0, 0.8f);
	// 8 SlewVoltages
	SlewVoltage* knobs[8] = {
		new SlewVoltage(knobLimit),
		new SlewVoltage(knobLimit),
		new SlewVoltage(knobLimit),
		new SlewVoltage(knobLimit),
		new SlewVoltage(knobLimit),
		new SlewVoltage(knobLimit),
		new SlewVoltage(knobLimit),
		new SlewVoltage(knobLimit)
	};


	G8Pad() {
		config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
		configOutput(GATE_OUTPUT, "");
		configOutput(VELOCITY_OUTPUT, "");
		configOutput(BEND_OUTPUT, "");
		configOutput(TOUCH_OUTPUT, "");
		configOutput(MOD_OUTPUT, "");
		configOutput(KNOB1_OUTPUT, "");
		configOutput(KNOB2_OUTPUT, "");
		configOutput(KNOB3_OUTPUT, "");
		configOutput(KNOB4_OUTPUT, "");
		configOutput(KNOB5_OUTPUT, "");
		configOutput(KNOB6_OUTPUT, "");
		configOutput(KNOB7_OUTPUT, "");
		configOutput(KNOBS8_OUTPUT, "");
		configLight(GATE_LIGHT, "");

		divider.setDivision(1024 * 4);
	}

	json_t* dataToJson() {
		json_t* rootJ = json_object();
		json_object_set_new(rootJ, "bendLimit", bendLimit->toJson());
		json_object_set_new(rootJ, "bend", bend->toJson());

		json_object_set_new(rootJ, "modLimit", modLimit->toJson());
		json_object_set_new(rootJ, "mod", mod->toJson());

		json_object_set_new(rootJ, "touchLimit", touchLimit->toJson());
		json_object_set_new(rootJ, "touch", touchLimit->toJson());


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
		json_object_set_new(rootJ, "knobVoltages", knobsJ);

		return rootJ;
	}

	void dataFromJson(json_t* rootJ) {
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

		// touch slew limit
		json_t* touchLimitJ = json_object_get(rootJ, "touchLimit");
		if (touchLimitJ) {
			touchLimit->fromJson(touchLimitJ);
		}
		// touch voltage
		json_t* touchJ = json_object_get(rootJ, "touch");
		if (touchJ) {
			touch->fromJson(touchJ, touchLimit);
		}

		// knob strength: int
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
		// 8 SlewVoltage->fromJson objects from knobs as json array
		json_t* knobsJ = json_object_get(rootJ, "knobVoltages");
		if (knobsJ) {
			for (int i = 0; i < 8; i++) {
				json_t* knobJ = json_array_get(knobsJ, i);
				if (knobJ) {
					knobs[i]->fromJson(knobJ, knobLimit);
				}
			}
		}
	}

	void processExpander() {
		if (leftExpander.module == NULL) {
			padId = -1;
			midiInput.setChannel(-1);
			midiInput.setDeviceId(-1);
			midiInput.setDriverId(-1);
			return;
		}

		if (leftExpander.module->model == modelMiniLab3) {
			padId = 1;
			MiniLab3* miniLab3 = dynamic_cast<MiniLab3*>(leftExpander.module);
			if (miniLab3) {
				midiInput.setDriverId(miniLab3->midiInput.driverId);
				midiInput.setDeviceId(miniLab3->midiInput.deviceId);
				midiInput.setChannel(miniLab3->midiInput.channel);
			}
		} else if (leftExpander.module->model == modelG8Pad) {
			G8Pad* leftPad = dynamic_cast<G8Pad*>(leftExpander.module);
			if (leftPad) {
				if (leftPad->padId >= 0) {
					padId = leftPad->padId + 1;
					int remoteDriverId = leftPad->midiInput.driverId;
					if (remoteDriverId >= 0 && remoteDriverId != midiInput.driverId) {
						INFO("G8Pad: Setting driverId for pad %d to %d", padId, remoteDriverId);						
						midiInput.setDriverId(remoteDriverId);
					}
					int remoteDeviceId = leftPad->midiInput.deviceId;
					if (remoteDeviceId >= 0 && remoteDeviceId != midiInput.deviceId) {
						INFO("G8Pad: Setting deviceId for pad %d to %d", padId, remoteDeviceId);
						midiInput.setDeviceId(remoteDeviceId);
					}
					int remoteChannel = leftPad->midiInput.channel;
					if (remoteChannel >= 0) {
						INFO("G8Pad: Setting channel for pad %d to %d", padId, remoteChannel);
						// midiInput.setChannel(remoteChannel);
					}
				} else {
					padId = -1;
					midiInput.setChannel(-1);
					midiInput.setDeviceId(-1);
					midiInput.setDriverId(-1);
				}
			}
		} else {
			padId = -1;
			midiInput.setChannel(-1);
			midiInput.setDeviceId(-1);
			midiInput.setDriverId(-1);
		}
	}

	int myNote() {
		switch (padId) {
			// C1
			case 1: return 36;
			// C2
			case 2: return 48;
			// C3
			case 3: return 60;
			// C4
			case 4: return 72;
			// C5
			case 5: return 84;
			// C6
			case 6: return 96;
			// C7
			case 7: return 108;
			// C8
			case 8: return 120;
			// D1
			case 9: return 38;
			// D2
			case 10: return 50;
			// D3
			case 11: return 62;
			// D4
			case 12: return 74;
			// D5
			case 13: return 86;
			// D6
			case 14: return 98;
			// D7
			case 15: return 110;
			// D8
			case 16: return 122;
			default: return -1;
		}
	}

	void process(const ProcessArgs& args) override {
		if (divider.process()) {
			processExpander();
		}

		midi::Message msg;
		while (midiInput.tryPop(&msg, args.frame)) {
			processMidi(msg);
		}

		if (!gateOpen) {
			bend->setTarget(64);
			touch->setTarget(0);			
		}

		outputs[BEND_OUTPUT].setVoltage(bend->getVoltage(args.sampleTime, args.sampleRate));
		outputs[MOD_OUTPUT].setVoltage(mod->getVoltage(args.sampleTime, args.sampleRate));
		outputs[TOUCH_OUTPUT].setVoltage(touch->getVoltage(args.sampleTime, args.sampleRate));
		for (int i = 0; i < 8; i++) {
			outputs[KNOB1_OUTPUT + i].setVoltage(knobs[i]->getVoltage(args.sampleTime, args.sampleRate));
		}

		// set light
		lights[GATE_LIGHT].setBrightness(gateOpen ? 1.0f : 0.0f);
	}

	void processMidi(midi::Message& msg) {
		int channel = msg.getChannel() + 1;

		switch (msg.getStatus()) {
			case 0x9: // note on
				if (channel == 16 && msg.getNote() == myNote()) {
					gateOpen = true;
					outputs[GATE_OUTPUT].setVoltage(10.0f);
					outputs[VELOCITY_OUTPUT].setVoltage(msg.getValue() / 127.0f * 10.0f);
				}
				break;
			case 0x8: // note off
				if (channel == 16 && msg.getNote() == myNote()) {
					gateOpen = false;
					outputs[GATE_OUTPUT].setVoltage(0.0f);
				}
				break;
			case 0xe: // pitch bend
				if (gateOpen) {
					processPitchBend(msg);
				}
				break;
			case 0xb: // control change
				if (gateOpen) {
					processControlChange(msg);
				}
				break;
			case 0xa: // aftertouch
				if (gateOpen) {
					processAftertouch(msg);
				}
				break;
			default:
				break;			
		}
	}	


	void processPitchBend(midi::Message& msg) {
		bend->setTarget(((uint16_t)msg.getValue() << 7) | msg.getNote());
	}

	void processAftertouch(midi::Message& msg) {
		int note = msg.getValue();
		touch->setTarget(note);
	}

	void processKnob(int knob, int value) {
		if (!gateOpen) {
			return;
		}
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

	void processControlChange(midi::Message& msg) {
		int note = msg.getNote();

		switch (note) {
			case 1:
				mod->setTarget(msg.getValue());
				break;
			case 102:
			case 103:
			case 104:
			case 105:
			case 106:
			case 107:
			case 108:
			case 109:
				processKnob(note - 102, msg.getValue());
				break;
		}
	}
};


struct G8PadWidget : ModuleWidget {
	LedTextDisplay *padIdText;
	
	SmallLight<GreenLight> *greenLight;

	G8PadWidget(G8Pad* module) {
		setModule(module);
		setPanel(createPanel(asset::plugin(pluginInstance, "res/G8Pad.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(15.189, 24.457)), module, G8Pad::GATE_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(9.803, 41.619)), module, G8Pad::TOUCH_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(20.677, 41.619)), module, G8Pad::VELOCITY_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(9.847, 58.734)), module, G8Pad::BEND_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(20.72, 58.734)), module, G8Pad::MOD_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(9.847, 78.694)), module, G8Pad::KNOB1_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(20.72, 78.694)), module, G8Pad::KNOB2_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(9.76, 91.276)), module, G8Pad::KNOB3_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(20.633, 91.276)), module, G8Pad::KNOB4_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(9.76, 103.857)), module, G8Pad::KNOB5_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(20.633, 103.857)), module, G8Pad::KNOB6_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(9.76, 116.438)), module, G8Pad::KNOB7_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(20.633, 116.439)), module, G8Pad::KNOBS8_OUTPUT));

		addChild(createLightCentered<SmallLight<GreenLight>>(mm2px(Vec(5, 8.894)), module, G8Pad::GATE_LIGHT));

		padIdText = createWidget<LedTextDisplay>(mm2px(Vec(21.5, 5)));
		padIdText->text = "--";
		addChild(padIdText);
	}

	void step() override {
		if (module) {
			G8Pad* pad = dynamic_cast<G8Pad*>(module);
			if (pad->padId >= 0) {
				padIdText->text = string::f("%02d", pad->padId);
			} else {
				padIdText->text = "--";
			}
		}
		ModuleWidget::step();
	}

	void appendContextMenu(Menu* menu) override {
		ModuleWidget::appendContextMenu(menu);

		// add menu slider for controlling bend slew
		G8Pad* module = dynamic_cast<G8Pad*>(this->module);
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


Model* modelG8Pad = createModel<G8Pad, G8PadWidget>("G8Pad");