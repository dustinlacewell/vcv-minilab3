#include <osdialog.h>
#include <iomanip>
#include <rack.hpp>

#include "plugin.hpp"
#include "ui/LedText.cpp"


const int BUFFERSIZE = 800;


struct MidiLog : Module {
	enum ParamId {
		PARAMS_LEN
	};
	enum InputId {
		INPUTS_LEN
	};
	enum OutputId {
		OUTPUTS_LEN
	};
	enum LightId {
		LIGHTS_LEN
	};

	MidiLog() {
		config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
		onReset();
	}

	midi::InputQueue midiInput;	
	dsp::RingBuffer<std::string, 512> midiLogMessages;

	uint64_t sample;

	/** [Stored to JSON] */
	bool showNoteMsg;
	/** [Stored to JSON] */
	bool showKeyPressure;
	/** [Stored to JSON] */
	bool showCcMsg;
	/** [Stored to JSON] */
	bool showProgChangeMsg;
	/** [Stored to JSON] */
	bool showChannelPressurelMsg;
	/** [Stored to JSON] */
	bool showPitchWheelMsg;

	/** [Stored to JSON] */
	bool showSysExMsg;
	/** [Stored to JSON] */
	bool showSysExData;
	/** [Stored to JSON] */
	bool showClockMsg;
	/** [Stored to JSON] */
	bool showSystemMsg;	

	void onReset() override {
		showNoteMsg = true;
		showKeyPressure = true;
		showCcMsg = true;
		showProgChangeMsg = true;
		showChannelPressurelMsg = true;
		showPitchWheelMsg = true;

		showSysExMsg = false;
		showSysExData = false;
		showClockMsg = false;
		showSystemMsg = true;

		midiLogMessages.push(string::f("RATE %i", int(APP->engine->getSampleRate())));
		Module::onReset();
	}

	void process(const ProcessArgs& args) override {
		midi::Message msg;
		while (midiInput.tryPop(&msg, args.frame)) {
			processMidi(msg);
		}
		sample++;
	}

	std::string padi(size_t n, int i) {
		std::string s = std::to_string(i);
		while (s.length() < n) {
			s = " " + s;
		}
		return s;
	}

	std::string pads(size_t n, std::string s) {
		while (s.length() < n) {
			s = " " + s;
		}
		return s;
	}

	void processMidi(midi::Message& msg) {
		// render channel as two digit number
		std::string chan = padi(2, msg.getChannel() + 1);
		// render note as a three digit number, without leading zeros
		std::string note = padi(3, msg.getNote());

		switch (msg.getStatus()) {
			case 0x9: // note on
				if (!midiLogMessages.full() && showNoteMsg) {
					uint8_t vel = msg.getValue();
					std::string s = string::f("%s %s %s | %i", chan.c_str(), pads(5, "ON").c_str(), note.c_str(), vel);
					midiLogMessages.push(s);
				} break;
			case 0x8: // note off
				if (!midiLogMessages.full() && showNoteMsg) {
					uint8_t vel = msg.getValue();
					std::string s = string::f("%s %s %s | %i", chan.c_str(), pads(5, "OFF").c_str(), note.c_str(), vel);
					midiLogMessages.push(s);
				} break;
			case 0xa: // key pressure
				if (!midiLogMessages.full() && showKeyPressure) {
					uint8_t value = msg.getValue();
					std::string s = string::f("%s %s %s | %i", chan.c_str(), pads(5, "PRESS").c_str(), note.c_str(), value);
					midiLogMessages.push(s);
				} break;
			case 0xb: // cc
				if (!midiLogMessages.full() && showCcMsg) {
					int8_t value = msg.bytes[2];
					std::string s = string::f("%s %s %s | %i", chan.c_str(), pads(5, "CC").c_str(), note.c_str(), value);
					midiLogMessages.push(s);
				} break;
			case 0xc: // program change
				if (!midiLogMessages.full() && showProgChangeMsg) {
					uint8_t prog = msg.getNote();
					std::string s = string::f("%s %s %i", chan.c_str(), pads(5, "PROG").c_str(), prog);
					midiLogMessages.push(s);
				} break;
			case 0xd: // channel pressure
				if (!midiLogMessages.full() && showChannelPressurelMsg) {
					uint8_t value = msg.getNote();
					std::string s = string::f("%s PRESS %i", chan.c_str(), value);
					midiLogMessages.push(s);
				} break;
			case 0xe: // pitch wheel
				if (!midiLogMessages.full() && showPitchWheelMsg) {
					uint16_t value = ((uint16_t)msg.getValue() << 7) | msg.getNote();
					std::string s = string::f("%s PITCH %i", chan.c_str(), value);
					midiLogMessages.push(s);
				} break;
			case 0xf: // system
				if (!midiLogMessages.full()) {
					switch (msg.getChannel()) {
						case 0x0: // sysex
							if (showSysExMsg) {
								std::string s = string::f("SYSEX (%i bytes)", msg.getSize());
								midiLogMessages.push(s);
								if (showSysExData) { // sysex bytes
									std::ostringstream ss;
									ss << std::hex;
									for (int i = 0; i < msg.getSize(); i++) {
										ss << std::setw(2) << std::setfill('0') << static_cast<int>(msg.bytes[i]) << " ";
									}
									midiLogMessages.push(ss.str());
								}
							} break;
						case 0x2: // song pointer
							if (showSystemMsg) {
								uint16_t value = ((uint16_t)msg.getValue() << 7) | msg.getNote();
								std::string s = string::f("POINTER %i", value);
								midiLogMessages.push(s);
							} break;
						case 0x3: // song select
							if (showSystemMsg) {
								uint8_t song = msg.getNote();
								std::string s = string::f("SONG %i", song);
								midiLogMessages.push(s);
							} break;
						case 0x8: // timing clock
							if (showClockMsg) {
								midiLogMessages.push("TICK");
							} break;
						case 0xa: // start
							if (showSystemMsg) {
								midiLogMessages.push("START");
							} break;
						case 0xb: // continue
							if (showSystemMsg) {
								midiLogMessages.push("CONTINUE");
							} break;
						case 0xc: // stop
							if (showSystemMsg) {
								midiLogMessages.push("STOP");
							} break;
						default:
							midiLogMessages.push("UNKOWN SYSTEM MSG");
							break;
					}
				}
				break;
			default:
				midiLogMessages.push("UNKOWN MSG");
				break;
		}
	}	
};



struct MidiTextLog : Widget {
	LedDisplay* background;
	LedTextDisplay* ledDisplay;

	std::list<std::string> buffer;

	MidiTextLog() {
		// background = createWidget<LedDisplay>(Vec(0.0, 0.0));
		// background->box.size = box.size;
		// addChild(background);

		ledDisplay = createWidget<LedTextDisplay>(Vec(0, 0));
		addChild(ledDisplay);
	}
	
	void step() override {
		// background->box.size = box.size;
		ledDisplay->box.size = box.size;
		Widget::step();
	}

	void bufferToString() {
		// set the text to messages joined by newline
		std::string s = "";
		for (std::string m : buffer) {
			s += m + "\n";
		}
		ledDisplay->text = s;
	}

	void push(std::string text) {
		buffer.push_back(text);

		if (buffer.size() > 19) {
			buffer.pop_front();
		}

		bufferToString();
	}


	void onButton(const event::Button& e) override {
		if (e.action == GLFW_PRESS && e.button == GLFW_MOUSE_BUTTON_RIGHT) {
			buffer.clear();
		}
		Widget::onButton(e);
		Widget::step();
	}
};

struct MidiLogWidget : ModuleWidget {

	MidiTextLog* log;
	bool dirty = true;

	MidiLogWidget(MidiLog* module) {
		setModule(module);
		setPanel(createPanel(asset::plugin(pluginInstance, "res/MidiLog.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		float controlsY = 13.f;

		MidiDisplay* display = createWidget<MidiDisplay>(mm2px(Vec(0.0, controlsY)));
		display->box.size = mm2px(Vec(40.64, 29.012));
		display->setMidiPort(module ? &module->midiInput : NULL);
		addChild(display);

		float logX = 0;
		float logY = display->box.pos.y + display->box.size.y;
		float logWidth = box.size.x;
		float logHeight = box.size.y - (logY + 2.f) - RACK_GRID_WIDTH;

		log = createWidget<MidiTextLog>(Vec(logX, logY));
		log->box.size = Vec(logWidth, logHeight);
		addChild(log);
	}

	void step() override {
		ModuleWidget::step();
		if (!module) {
			return;
		}

		MidiLog* midiLog = reinterpret_cast<MidiLog*>(this->module);
		while (!midiLog->midiLogMessages.empty()) {
			std::string s = midiLog->midiLogMessages.shift();
			log->push(s);
		}
	}	
};




Model* modelMidiLog = createModel<MidiLog, MidiLogWidget>("MidiLog");