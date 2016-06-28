#pragma once

#include "juce_audio_formats/juce_audio_formats.h"
#include "juce_audio_devices/juce_audio_devices.h"

class DefaultAudioFormatManager : public juce::AudioFormatManager {
public:
    DefaultAudioFormatManager();
};

class DefaultAudioDeviceManager : public juce::AudioDeviceManager {
public:
    DefaultAudioDeviceManager();
};
