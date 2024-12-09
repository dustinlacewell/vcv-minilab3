# MiniLab3 - Developer's Guide

## Overview

The MiniLab3 Multiplexer is a VCV Rack module suite that provides multi-module control for the Arturia MiniLab3 controller. It consists of three module types that work together:

- MiniLab: Main module handling knobs and sliders
- MiniPad: Pad-specific modules for individual pad control
- MiniLog: Diagnostic module for MIDI message monitoring

## Core Architecture

### Module Coordination

Rather than routing MIDI through a central module, each module maintains its own MIDI input connection. The modules coordinate through VCV's expander mechanism to create a cohesive control surface:

```cpp
void MiniPad::process(const ProcessArgs& args) {
    if (positionDivider.process()) {
        Module* current = leftExpander.module;
        
        // Scan left for MiniLab to determine position
        int pos = 0;
        bool found = false;
        bool labReady = false;
        
        while (current && !found) {
            auto miniLab = dynamic_cast<MiniLab*>(current);
            if (miniLab) {
                found = true;
                labReady = miniLab->isReady;
                break;
            }
            pos++;
            current = current->leftExpander.module;
        }
        
        // Update state based on position
        position = pos;
        dirty = found != isReady;
        isReady = found;
    }
}
```

The modules use clock dividers to efficiently manage different update rates:

```cpp
MiniPad::MiniPad() : BaseModule() {
    driverDivider.setDivision(4096);  // Slow driver scanning
    positionDivider.setDivision(1024); // Position updates
    midiDivider.setDivision(16);      // MIDI processing
    paramDivider.setDivision(256);    // Parameter updates
}
```

### Parameter System

The parameter system is built around a flexible hierarchy that supports different control modes:

#### Base Parameter

The BaseParam class provides core functionality:

```cpp
struct BaseParam {
protected:
    std::string name;
    engine::Output* output;
    std::unique_ptr<Pile> pile;
    std::unique_ptr<Clamp<int>> clamp;
    std::unique_ptr<VoltageRescaler> rescaler;
    std::unique_ptr<Slew> slew;
    
    // Value change notification system
    std::vector<std::function<void(float)>> valueChangeCallbacks;
};
```

Key features:
- Value clamping
- Voltage rescaling
- Slew limiting
- Callback system for value changes
- JSON serialization

#### Parameter Types

Two concrete parameter types extend BaseParam:
- AbsoluteParam: Direct value control, typically used for knobs and sliders
- RelativeParam: Incremental control with configurable behavior:

```cpp
struct RelativeParam : BaseParam {
    std::unique_ptr<StrengthChoice> strengthChoice;
    std::unique_ptr<ResolutionChoice> resolutionChoice;
    
    void send(int value) override {
        // Complex relative value processing
        // See RelativeParam.cpp for implementation
    }
};
```

### Configuration System

The configuration system uses a template-based approach for handling different types of choices:

```cpp
template <typename T>
struct Choice {
    T value;
    std::string label;
    std::function<void(T)> callback;
    
    virtual std::vector<std::tuple<std::string, T>> getChoices() = 0;
};
```

Specific configurations include:

- Resolution:
```cpp
enum Resolution { 
    Two, Three, Four, Five, Eight, Twelve, 
    Low, Medium, High 
};
```

- Voltage Modes:
```cpp
enum VoltageMode {
    UNIPOLAR_1, UNIPOLAR_5, UNIPOLAR_10,
    BIPOLAR_1, BIPOLAR_5, BIPOLAR_10
};
```

- Strength Levels:
```cpp
enum Strength {
    OneTick, TwoTicks, TenTicks,
    OnePercent, TwoPercent, FivePercent,
    TenPercent, TwentyPercent, FiftyPercent,
    Toggle
};
```

### Widget System

The widget system uses a sophisticated template-based approach for UI construction:

```cpp
template <typename T, typename W>
struct BaseWidget : ModuleWidget, SvgHelper<W> {
    FancyPanelBorder* fancyPanelBorder;
    
    void updateBorders(
        std::vector<Model*> leftModels,
        std::vector<Model*> rightModels
    ) {
        // Dynamic border handling for connected modules
    }
};
```

#### SVG Integration

The SvgHelper provides powerful SVG element location:

```cpp
std::optional<Vec> findNamed(std::string name) {
    std::optional<Vec> result;
    
    forEachShape([&](NSVGshape* shape) {
        if (std::string(shape->id) == name) {
            auto bounds = shape->bounds;
            result = Vec(
                (bounds[0] + bounds[2]) / 2,
                (bounds[1] + bounds[3]) / 2
            );
        }
    });
    
    return result;
}
```

### MIDI Processing

Each module independently processes MIDI messages through a filtering and rendering pipeline:

```cpp
void MiniLog::processMessage(midi::Message& msg) {
    if (messages.full()) return;
    
    if (midiMessageFilter.shouldHandle(msg)) {
        std::string renderedMsg = midiMessageRenderer.render(msg);
        messages.push(renderedMsg);
    }
}
```

The MidiMessageRenderer provides detailed message formatting:

```cpp
std::string MidiMessageRenderer::render(midi::Message& msg) {
    std::string chan = padi(2, msg.getChannel() + 1);
    std::string note = padi(3, msg.getNote());
    
    switch (msg.getStatus()) {
        case NoteOn:
            return renderNoteOn(msg, chan, note);
        case NoteOff:
            return renderNoteOff(msg, chan, note);
        // ... other message types
    }
}
```

## State Management

### Parameter State

Parameters maintain their state through a combination of:

```cpp
json_t* BaseParam::toJson() {
    json_t* rootJ = json_object();
    json_object_set_new(rootJ, "pile", pile->toJson());
    json_object_set_new(rootJ, "clamp", clamp->toJson());
    json_object_set_new(rootJ, "rescaler", rescaler->toJson());
    json_object_set_new(rootJ, "slew", slew->toJson());
    return rootJ;
}
```

### Module Coordination State

Modules track their relationships and status:

```cpp
struct MiniPad : BaseModule {
    int driverId = -1;
    int deviceId = -1;
    int channel = -1;
    bool isReady = false;
    bool dirty = false;
};
```

## Extension Points

### Adding New Parameters

1. Create a new parameter class:

```cpp
struct NewParam : BaseParam {
    // Custom parameter behavior
};
```

2. Add menu support:

```cpp
struct NewParamMenu : ModularContextMenuItem {
    void appendContextMenu(Menu* menu) override {
        // Menu construction
    }
};
```

### Adding New MIDI Message Types

1. Update MidiMessageFilter:

```cpp
bool MidiMessageFilter::shouldHandle(midi::Message msg) {
    switch (msg.getStatus()) {
        case NewMessageType:
            return showNewMessageType;
        // ... existing cases
    }
}
```

2. Add rendering support in MidiMessageRenderer

### Module Communication

To add new inter-module features:
1. Define shared state in base module
2. Implement scanning in process() methods
3. Add coordination logic in relevant modules

## Best Practices

### Performance Considerations

- Use appropriate clock divider rates for different operations
- Batch parameter updates where possible
- Minimize dynamic casting in hot paths

### Memory Management

- Use smart pointers for parameter ownership
- Clean up expander connections properly
- Handle module deletion gracefully

### Error Handling

- Validate MIDI message contents
- Check for null modules in expander slots
- Verify parameter ranges

This architecture provides a robust foundation for MIDI control surface implementation while maintaining flexibility for future extensions.