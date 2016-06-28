#pragma once

#include "juce_audio_devices/juce_audio_devices.h"
#include "juce_events/juce_events.h"
#include "juce_gui_basics/juce_gui_basics.h"

#include <memory>

class TransportDisplay : public juce::Component, public juce::Timer {
public:
    TransportDisplay(juce::AudioTransportSource& audioTransportSource);
    virtual ~TransportDisplay() noexcept = default;

    void paint(juce::Graphics& g) override;
    void resized() override;

    void timerCallback() override;

private:
    juce::AudioTransportSource& audioTransportSource;

    juce::Label label;
};