#pragma once

#include <iomanip>
#include "../plugin.hpp"

struct MidiMessageRenderer {
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
	
	std::string renderSysexMessage(midi::Message& msg) {
		std::string s = string::f("SYSEX (%i bytes)", msg.getSize());
		return s;
		std::ostringstream ss;
		ss << std::hex;
		for (int i = 0; i < msg.getSize(); i++) {
			ss << std::setw(2) << std::setfill('0') << static_cast<int>(msg.bytes[i]) << " ";
		}
		return ss.str();
	}
	
	std::string renderSongPointerMessage(midi::Message& msg) {
		uint16_t value = ((uint16_t)msg.getValue() << 7) | msg.getNote();
		std::string s = string::f("POINTER %i", value);
		return s;
	}
	
	std::string renderSongSelectMessage(midi::Message& msg) {
		uint8_t song = msg.getNote();
		std::string s = string::f("SONG %i", song);
		return s;
	}
	
	std::string renderTimingClockMessage(midi::Message& msg) {
		return "TICK";
	}
	
	std::string renderStartMessage(midi::Message& msg) {
		return "START";
	}
	
	std::string renderContinueMessage(midi::Message& msg) {
		return "CONTINUE";
	}
	
	std::string renderStopMessage(midi::Message& msg) {
		return "STOP";
	}

	std::string renderNoteOn(midi::Message& msg, std::string chan, std::string note) {
		uint8_t vel = msg.getValue();
		std::string s = string::f("%s %s %s | %i", chan.c_str(), pads(5, "ON").c_str(), note.c_str(), vel);
		return s;
	}

	std::string renderNoteOff(midi::Message& msg, std::string chan, std::string note) {
		uint8_t vel = msg.getValue();
		std::string s = string::f("%s %s %s | %i", chan.c_str(), pads(5, "OFF").c_str(), note.c_str(), vel);
		return s;
	}

	std::string renderKeyPressure(midi::Message& msg, std::string chan, std::string note) {
		uint8_t value = msg.getValue();
		std::string s = string::f("%s %s %s | %i", chan.c_str(), pads(5, "TOUCH").c_str(), note.c_str(), value);
		return s;
	}

	std::string renderCC(midi::Message& msg, std::string chan, std::string note) {
		int8_t value = msg.bytes[2];
		std::string s = string::f("%s %s %s | %i", chan.c_str(), pads(5, "CC").c_str(), note.c_str(), value);
		return s;
	}

	std::string renderProgramChange(midi::Message& msg, std::string chan) {
		uint8_t prog = msg.getNote();
		std::string s = string::f("%s %s %i", chan.c_str(), pads(5, "PROG").c_str(), prog);
		return s;
	}

	std::string renderChannelPressure(midi::Message& msg, std::string chan) {
		uint8_t value = msg.getNote();
		std::string s = string::f("%s PRESS %i", chan.c_str(), value);
		return s;
	}

	std::string renderPitchWheel(midi::Message& msg, std::string chan) {
		uint16_t value = ((uint16_t)msg.getValue() << 7) | msg.getNote();
		std::string s = string::f("%s PITCH %i", chan.c_str(), value);
		return s;
	}

	std::string renderSystemMessage(midi::Message& msg) {
		switch (msg.getChannel()) {
			case 0x0: // sysex
				return renderSysexMessage(msg);
				break;
			case 0x2: // song pointer
				return renderSongPointerMessage(msg);
				break;
			case 0x3: // song select
				return renderSongSelectMessage(msg);
				break;
			case 0x8: // timing clock
				return renderTimingClockMessage(msg);
				break;
			case 0xa: // start
				return renderStartMessage(msg);
				break;
			case 0xb: // continue
				return renderContinueMessage(msg);
				break;
			case 0xc: // stop
				return renderStopMessage(msg);
				break;
			default:
				return "UNKOWN SYSTEM MSG";
				break;
		}		
	}    

	std::string render(midi::Message& msg) {
		// render channel as two digit number
		std::string chan = padi(2, msg.getChannel() + 1);
		// render note as a three digit number, without leading zeros
		std::string note = padi(3, msg.getNote());

		switch (msg.getStatus()) {
			case 0x9: // note on
				return renderNoteOn(msg, chan, note);
				break;
			case 0x8: // note off
				return renderNoteOff(msg, chan, note);
				break;
			case 0xa: // key pressure
				return renderKeyPressure(msg, chan, note);
				break;
			case 0xb: // cc
				return renderCC(msg, chan, note);
				break;
			case 0xc: // program change
				return renderProgramChange(msg, chan);
				break;
			case 0xd: // channel pressure
				return renderChannelPressure(msg, chan);
				break;
			case 0xe: // pitch wheel
				return renderPitchWheel(msg, chan);
				break;
			case 0xf: // system
				return renderSystemMessage(msg);
				break;
			default:
				return "UNKOWN MSG";
				break;
		}
	}    	
};