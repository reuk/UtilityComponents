#pragma once

#include "TransportDisplay.hpp"

class Transport : public Component,
                  public Button::Listener,
                  public ChangeListener {
public:
    Transport(AudioTransportSource &transportSource);
    virtual ~Transport() noexcept = default;

    void resized() override;

    void buttonClicked(Button *b) override;
    void changeListenerCallback(ChangeBroadcaster *source) override;

private:
    AudioTransportSource &transport_source;

    TransportDisplay transport_display;
    DrawableButton rewind_button;
    DrawableButton play_button;
};