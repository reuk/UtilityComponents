#pragma once

#include "TransportDisplay.hpp"

class Transport : public Component,
                  public Button::Listener,
                  public ChangeListener {
public:
    class Listener {
    public:
        Listener() = default;
        Listener(const Listener &) = default;
        Listener &operator=(const Listener &) = default;
        Listener(Listener &&) noexcept = default;
        Listener &operator=(Listener &&) noexcept = default;
        virtual ~Listener() noexcept = default;

        virtual void transport_play(Transport *) = 0;
        virtual void transport_pause(Transport *) = 0;
        virtual void transport_rewind(Transport *) = 0;
    };

    Transport(AudioTransportSource &transportSource);
    virtual ~Transport() noexcept = default;

    void resized() override;

    void buttonClicked(Button *b) override;
    void changeListenerCallback(ChangeBroadcaster *source) override;

    void addListener(Listener *l);
    void removeListener(Listener *l);

private:
    const AudioTransportSource &transportSource;

    TransportDisplay transport_display;
    DrawableButton rewind_button;
    DrawableButton play_button;

    ListenerList<Listener> listener_list;
};