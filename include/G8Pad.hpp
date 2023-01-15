#pragma once

#include <rack.hpp>

#include "plugin.hpp"
#include "MiniLab3.hpp"
#include "ui/LedText.hpp"
#include "utils/Relay.hpp"
#include "utils/RelayCallback.hpp"
#include "menu/MenuSlider.hpp"


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

	G8Pad();

	json_t* dataToJson();

	void dataFromJson(json_t* rootJ);

	void processExpander();

	int myNote();

	void process(const ProcessArgs& args) override;

	void processMidi(midi::Message& msg);

	void processPitchBend(midi::Message& msg);

	void processAftertouch(midi::Message& msg);

	void processKnob(int knob, int value);

	void processControlChange(midi::Message& msg);
};