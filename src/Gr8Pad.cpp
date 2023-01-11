#include "plugin.hpp"


struct Gr8Pad : Module {
	/**
	 * This module takes four inputs, a gate, velocity, after-touch and pitch-bend.
	 * 
	 * When the gate is opened, the velocity is sampled and sent to all five outputs.
	 * The after-touch and pitch-bend are sent to all five outputs.
	 * 
	 * When the gate is closed, the after-touch and pitch-bend are sampled.
	 * The after-touch is interpolated to 0.
	 * The pitch-bend is interpolated to 0.5.
	 * 
	*/

	bool open = false;
	float sampledVelocity = 0.0f; // velocity on gate open
	float sampledTouch = 0.0f; // touch on gate close
	float sampledBend = 0.5f; // bend on gate close

	enum ParamId {
		PARAMS_LEN
	};
	enum InputId {
		GATE_INPUT,
		VELOCITY_INPUT,
		BEND_INPUT,
		TOUCH_INPUT,
		INPUTS_LEN
	};
	enum OutputId {
		VELOCITY1_OUTPUT,
		TOUCH1_OUTPUT,
		BEND1_OUTPUT,
		VELOCITY2_OUTPUT,
		TOUCH2_OUTPUT,
		BEND2_OUTPUT,
		VELOCITY3_OUTPUT,
		TOUCH3_OUTPUT,
		BEND3_OUTPUT,
		VELOCITY4_OUTPUT,
		TOUCH4_OUTPUT,
		BEND4_OUTPUT,
		VELOCITY5_OUTPUT,
		TOUCH5_OUTPUT,
		BEND5_OUTPUT,
		OUTPUTS_LEN
	};
	enum LightId {
		LIGHT_LIGHT,
		LIGHTS_LEN
	};

	Gr8Pad() {
		config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
		configInput(GATE_INPUT, "");
		configInput(VELOCITY_INPUT, "");
		configInput(BEND_INPUT, "");
		configInput(TOUCH_INPUT, "");
		configOutput(VELOCITY1_OUTPUT, "");
		configOutput(TOUCH1_OUTPUT, "");
		configOutput(BEND1_OUTPUT, "");
		configOutput(VELOCITY2_OUTPUT, "");
		configOutput(TOUCH2_OUTPUT, "");
		configOutput(BEND2_OUTPUT, "");
		configOutput(VELOCITY3_OUTPUT, "");
		configOutput(TOUCH3_OUTPUT, "");
		configOutput(BEND3_OUTPUT, "");
		configOutput(VELOCITY4_OUTPUT, "");
		configOutput(TOUCH4_OUTPUT, "");
		configOutput(BEND4_OUTPUT, "");
		configOutput(VELOCITY5_OUTPUT, "");
		configOutput(TOUCH5_OUTPUT, "");
		configOutput(BEND5_OUTPUT, "");
	}

	void setVelocityOutput(float velocity) {
		outputs[VELOCITY1_OUTPUT].setVoltage(velocity);
		outputs[VELOCITY2_OUTPUT].setVoltage(velocity);
		outputs[VELOCITY3_OUTPUT].setVoltage(velocity);
		outputs[VELOCITY4_OUTPUT].setVoltage(velocity);
		outputs[VELOCITY5_OUTPUT].setVoltage(velocity);
	}

	void setTouchOutput(float touch) {
		outputs[TOUCH1_OUTPUT].setVoltage(touch);
		outputs[TOUCH2_OUTPUT].setVoltage(touch);
		outputs[TOUCH3_OUTPUT].setVoltage(touch);
		outputs[TOUCH4_OUTPUT].setVoltage(touch);
		outputs[TOUCH5_OUTPUT].setVoltage(touch);
	}

	void setBendOutput(float bend) {
		outputs[BEND1_OUTPUT].setVoltage(bend);
		outputs[BEND2_OUTPUT].setVoltage(bend);
		outputs[BEND3_OUTPUT].setVoltage(bend);
		outputs[BEND4_OUTPUT].setVoltage(bend);
		outputs[BEND5_OUTPUT].setVoltage(bend);
	}

	void process(const ProcessArgs& args) override {
		/**
		 * Gate initially opened:
		 * 	- sample velocity
		 * 	- send sampled velocity, in touch and in bend to all outputs
		 * 
		 * Gate initially closed:
		 * 	- sample touch and bend
		 * 	- send sampled velocity, touch and bend to all outputs
		 * 
		 * Gate closed:
		 * 	- interpolate sampled touch to 0.0
		 * 	- interpolate sampled bend to 0.5
		 *  - send sampled velocity to all outputs
		 * 	- send interpolated touch and bend to all outputs
		 * 
		 * Touch and Bend are interpolated to 0.0 and 0.5 respectively:
		 * 	- send sampled velocity to all outputs
		 *  - send interpolated touch and bend to all outputsthat
		 * 
		 * 
		*/


		float touch = 0.0f;
		float bend = 0.5f;

		bool connected = inputs[GATE_INPUT].isConnected();
		bool opened = connected && inputs[GATE_INPUT].getVoltage() > 0.0f;
		bool opening = opened && !open;
		bool closing = !opened && open;

		if (opening) {
			sampledVelocity = inputs[VELOCITY_INPUT].getVoltage();
		}

		if (closing) {
			sampledTouch = inputs[TOUCH_INPUT].getVoltage();
			sampledBend = inputs[BEND_INPUT].getVoltage();
		}

		if (!open && !closing) {

		}


	}
};


struct Gr8PadWidget : ModuleWidget {
	Gr8PadWidget(Gr8Pad* module) {
		setModule(module);
		setPanel(createPanel(asset::plugin(pluginInstance, "res/Gr8Pad.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(15.24, 20.45)), module, Gr8Pad::GATE_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(5.188, 30.426)), module, Gr8Pad::VELOCITY_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(25.155, 30.426)), module, Gr8Pad::BEND_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(15.24, 40.664)), module, Gr8Pad::TOUCH_INPUT));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(5.211, 65.911)), module, Gr8Pad::VELOCITY1_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(15.24, 65.911)), module, Gr8Pad::TOUCH1_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(25.421, 65.911)), module, Gr8Pad::BEND1_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(5.211, 76.87)), module, Gr8Pad::VELOCITY2_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(15.24, 76.87)), module, Gr8Pad::TOUCH2_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(25.421, 76.87)), module, Gr8Pad::BEND2_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(5.211, 87.829)), module, Gr8Pad::VELOCITY3_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(15.24, 87.829)), module, Gr8Pad::TOUCH3_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(25.421, 87.829)), module, Gr8Pad::BEND3_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(5.211, 98.788)), module, Gr8Pad::VELOCITY4_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(15.24, 98.788)), module, Gr8Pad::TOUCH4_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(25.421, 98.788)), module, Gr8Pad::BEND4_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(5.211, 109.747)), module, Gr8Pad::VELOCITY5_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(15.24, 109.747)), module, Gr8Pad::TOUCH5_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(25.421, 109.747)), module, Gr8Pad::BEND5_OUTPUT));

		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(15.24, 30.426)), module, Gr8Pad::LIGHT_LIGHT));
	}
};


Model* modelGr8Pad = createModel<Gr8Pad, Gr8PadWidget>("Gr8Pad");