#pragma once

#include <rack.hpp>
#include "VoltageMode.hpp"

class Slew {
public:
    explicit Slew(float limit);
    Slew(float limit, VoltageMode voltageMode);
    
    // Prevent implicit copying/moving
    Slew(const Slew&) = delete;
    Slew& operator=(const Slew&) = delete;
    Slew(Slew&&) = delete;
    Slew& operator=(Slew&&) = delete;

    // Inline accessors defined in header
    [[nodiscard]] inline VoltageMode getVoltageMode() const noexcept {
        return voltageMode;
    }
    
    inline void setVoltageMode(VoltageMode newVoltageMode) noexcept {
        voltageMode = newVoltageMode;
    }
    
    [[nodiscard]] inline float getLimit() const noexcept {
        return limit;
    }
    
    inline void setLimit(float newLimit) noexcept {
        limit = newLimit;
    }
    
    [[nodiscard]] inline float getTarget() const noexcept {
        return target;
    }
    
    inline void setTarget(float newTarget) noexcept {
        target = newTarget;
    }
    
    [[nodiscard]] float getSlewed(float sampleTime) noexcept;
    
    // Access to current output value
    [[nodiscard]] inline float getCurrentOutput() const noexcept {
        return slewLimiter.out;
    }
    
    inline float& getCurrentOutput() noexcept {
        return slewLimiter.out;
    }

    // JSON serialization
    [[nodiscard]] json_t* toJson() const;
    void fromJson(json_t* const rootJ);

private:
    float limit;
    float target{0.f};
    VoltageMode voltageMode;
    rack::dsp::SlewLimiter slewLimiter;
};