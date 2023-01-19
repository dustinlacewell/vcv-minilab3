#include <osdialog.h>
#include <iomanip>
#include <rack.hpp>

#include "plugin.hpp"
#include <MiniLab3.hpp>
#include <ui/TextLogWidget.hpp>
#include <utils/MidiMessageFilter.hpp>
#include <utils/MidiMessageRenderer.hpp>
#include <utils/Relay.hpp>
#include <utils/RelayCallback.hpp>
#include <utils/ExpanderRelay.hpp>


const int BUFFERSIZE = 800;


struct MidiLogExt : Module {
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
        CONNECTED_LIGHT, LIGHTS_LEN
    };

    MidiMessageFilter midiMessageFilter;
    MidiMessageRenderer midiMessageRenderer;

    // reference to MiniLab3 right expander
    MiniLab3 *miniLab3 = NULL;

    ExpanderRelay<MiniLab3 *> *expanderRelay = NULL;
    RelayCallback<MiniLab3 *> *connectedCallback = NULL;
    RelayCallback<MiniLab3 *> *disconnectedCallback = NULL;
    RelayCallback<midi::Message &> *midiCallback = NULL;
    RelayCallback<bool> *statusCallback = NULL;

    dsp::RingBuffer<std::string, 512> messages;

    MidiLogExt() {
        config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
        configLight(CONNECTED_LIGHT, "Connected");

        midiCallback = new RelayCallback<midi::Message &>(std::bind(&MidiLogExt::processMessage, this, std::placeholders::_1));
        statusCallback = new RelayCallback<bool>(std::bind(&MidiLogExt::adjustLight, this, std::placeholders::_1));

        connectedCallback = new RelayCallback<MiniLab3 *>(std::bind(&MidiLogExt::expanderConnected, this, std::placeholders::_1));
        disconnectedCallback = new RelayCallback<MiniLab3 *>(std::bind(&MidiLogExt::expanderDisconnected, this, std::placeholders::_1));

        expanderRelay = new ExpanderRelay<MiniLab3 *>(modelMiniLab3, true);
        connectedCallback->registerCallback(&(expanderRelay->connectedRelay));
        disconnectedCallback->registerCallback(&(expanderRelay->disconnectedRelay));

        onReset();
    }

    void onReset() override {
        midiMessageFilter.reset();
        messages.push(string::f("RATE %i", int(APP->engine->getSampleRate())));
        Module::onReset();
    }

    void processMessage(midi::Message &msg) {
        if (messages.full()) {
            return;
        }
        if (midiMessageFilter.shouldHandle(msg)) {
            std::string renderedMsg = midiMessageRenderer.render(msg);
            messages.push(renderedMsg);
        }
    }

    void adjustLight(bool connected) {
        lights[CONNECTED_LIGHT].setBrightness(connected ? 1.0 : 0.0);
    }

    void onExpanderChange(const ExpanderChangeEvent &e) override {
        expanderRelay->onExpanderChange(e, leftExpander, rightExpander);
    }

    void disconnectMiniLab3() {
        if (miniLab3) {
            midiCallback->unregisterCallback();
            statusCallback->unregisterCallback();
            miniLab3 = NULL;
        }
    }

    void expanderConnected(MiniLab3 *miniLab3) {
        disconnectMiniLab3();
        this->miniLab3 = miniLab3;
        midiCallback->registerCallback(&miniLab3->midiRelay);
        statusCallback->registerCallback(&miniLab3->midiInput.deviceStatusRelay);
        bool ready = miniLab3->midiInput.deviceId >= 0;
        adjustLight(ready);
    }

    void expanderDisconnected(MiniLab3 *miniLab3) {
        disconnectMiniLab3();
        adjustLight(false);
    }
};


struct MidiLogExtWidget : ModuleWidget {

    TextLogWidget *log;
    bool dirty = true;

    MidiLogExtWidget(MidiLogExt *
    module) {
        setModule(module);
        setPanel(createPanel(asset::plugin(pluginInstance, "res/MidiLog.svg")));

        addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
        addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
        addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
        addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
        addChild(createLight<SmallLight<GreenLight>>(mm2px(Vec(31.233, 7.894)), module, MidiLogExt::CONNECTED_LIGHT));

        log = createWidget<TextLogWidget>(mm2px(Vec(0, 13.f)));
        log->box.size = mm2px(Vec(40.64, 128.5f - 13.f - 5.f));
        addChild(log);
    }

    void step() override {
        ModuleWidget::step();
        if (!module) {
            return;
        }

        MidiLogExt *midiLog = reinterpret_cast<MidiLogExt *>(this->module);
        while (!midiLog->messages.empty()) {
            std::string s = midiLog->messages.shift();
            log->push(s);
        }
    }
};


Model *modelMidiLogExt = createModel<MidiLogExt, MidiLogExtWidget>("MidiLogExt");