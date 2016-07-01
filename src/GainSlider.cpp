#include "UtilityComponents/GainSlider.hpp"

#include "juce_audio_basics/juce_audio_basics.h"

GainSlider::GainSlider(Orientation orientation)
        : slider(orientation == Orientation::horizontal
                         ? juce::Slider::SliderStyle::LinearHorizontal
                         : juce::Slider::SliderStyle::LinearVertical,
                 juce::Slider::TextEntryBoxPosition::NoTextBox) {
    slider.setRange(-100, 0);
    slider.setPopupDisplayEnabled(true, nullptr);
    slider.setDoubleClickReturnValue(true, 0);
    slider.setValue(0, juce::NotificationType::dontSendNotification);
    addAndMakeVisible(slider);
    slider.addListener(this);
}

GainSlider::~GainSlider() noexcept {
    slider.removeListener(this);
}

void GainSlider::resized() {
    slider.setBounds(getLocalBounds());
}

void GainSlider::sliderValueChanged(juce::Slider* s) {
    if (s == &slider) {
        listener_list.call(&Listener::gain_slider_value_changed, this);
    }
}

void GainSlider::sliderDragStarted(juce::Slider* s) {
    if (s == &slider) {
        listener_list.call(&Listener::gain_slider_drag_started, this);
    }
}

void GainSlider::sliderDragEnded(juce::Slider* s) {
    if (s == &slider) {
        listener_list.call(&Listener::gain_slider_drag_ended, this);
    }
}

double GainSlider::get_gain() const {
    return juce::Decibels::decibelsToGain(slider.getValue());
}

void GainSlider::set_gain(double g, juce::NotificationType n) {
    slider.setValue(juce::Decibels::gainToDecibels(g), n);
}

void GainSlider::addListener(Listener* l) {
    listener_list.add(l);
}

void GainSlider::removeListener(Listener* l) {
    listener_list.remove(l);
}
