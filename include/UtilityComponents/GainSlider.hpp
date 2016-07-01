#pragma once

#include "juce_gui_basics/juce_gui_basics.h"

class GainSlider : public juce::Component, public juce::Slider::Listener {
public:
    class Listener {
    public:
        Listener() = default;
        Listener(const Listener&) = default;
        Listener& operator=(const Listener&) = default;
        Listener(Listener&&) noexcept = default;
        Listener& operator=(Listener&&) noexcept = default;

        virtual void gain_slider_value_changed(GainSlider*) = 0;
        virtual void gain_slider_drag_started(GainSlider*) = 0;
        virtual void gain_slider_drag_ended(GainSlider*) = 0;
    };

    enum class Orientation { horizontal, vertical };

    GainSlider(Orientation orientation);
    virtual ~GainSlider() noexcept;

    void resized() override;

    void sliderValueChanged(juce::Slider*) override;
    void sliderDragStarted(juce::Slider*) override;
    void sliderDragEnded(juce::Slider*) override;

    void addListener(Listener* l);
    void removeListener(Listener* l);

private:
    juce::ListenerList<Listener> listener_list;
    juce::Slider slider;
};
