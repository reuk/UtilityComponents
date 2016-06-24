#include "UtilityComponents/Transport.hpp"

Transport::Transport(AudioTransportSource &transport_source)
        : transport_source(transport_source)
        , transport_display(transport_source)
        , rewind_button("", DrawableButton::ImageFitted)
        , play_button("", DrawableButton::ImageFitted) {
    transport_source.addChangeListener(this);

    auto strokeWeight = 4;
    DrawableImage rewindDI;
    {
        Image rewindImage(Image::RGB, 40, 40, true);

        Graphics g(rewindImage);
        g.setFillType(FillType(ColourGradient(
                Colours::white, 0, 0, Colours::lightgrey, 0, 40, false)));

        Path p;
        p.addTriangle(40, 0, 40, 40, 0, 20);
        p.addLineSegment(Line<float>(0, 0, 0, 40), strokeWeight);

        g.fillPath(p);

        rewindDI.setImage(rewindImage);
    }
    rewind_button.setImages(&rewindDI);

    DrawableImage playDI;
    {
        Image playImage(Image::RGB, 40, 40, true);

        Graphics g(playImage);
        g.setFillType(FillType(ColourGradient(
                Colours::white, 0, 0, Colours::lightgrey, 0, 40, false)));

        Path p;
        p.addTriangle(0, 0, 0, 40, 40, 20);

        g.fillPath(p);

        playDI.setImage(playImage);
    }

    DrawableImage pauseDI;
    {
        Image pauseImage(Image::RGB, 40, 40, true);

        Graphics g(pauseImage);
        g.setFillType(FillType(ColourGradient(
                Colours::white, 0, 0, Colours::lightgrey, 0, 40, false)));

        Path p;
        p.addRectangle(0, 0, 16, 40);
        p.addRectangle(24, 0, 16, 40);

        g.fillPath(p);

        pauseDI.setImage(pauseImage);
    }
    play_button.setImages(&playDI, nullptr, nullptr, nullptr, &pauseDI);

    play_button.setClickingTogglesState(true);
    play_button.setColour(DrawableButton::backgroundColourId,
                          Colours::transparentBlack);
    play_button.setColour(DrawableButton::backgroundOnColourId,
                          Colours::transparentBlack);

    rewind_button.addListener(this);
    play_button.addListener(this);

    transport_display.setWantsKeyboardFocus(false);
    rewind_button.setWantsKeyboardFocus(false);
    play_button.setWantsKeyboardFocus(false);

    addAndMakeVisible(transport_display);
    addAndMakeVisible(rewind_button);
    addAndMakeVisible(play_button);
}

void Transport::resized() {
    auto padding = 8;
    auto buttonHeight = getHeight() - padding * 2;

    transport_display.setSize(100, buttonHeight);
    rewind_button.setSize(buttonHeight, buttonHeight);
    play_button.setSize(buttonHeight, buttonHeight);

    transport_display.setTopLeftPosition(padding, padding);
    rewind_button.setTopLeftPosition(transport_display.getRight() + padding,
                                     padding);
    play_button.setTopLeftPosition(rewind_button.getRight() + padding, padding);
}

void Transport::buttonClicked(Button *b) {
    if (b == &rewind_button) {
        transport_source.setPosition(0);
    } else if (b == &play_button) {
        if (b->getToggleState()) {
            transport_source.start();
        } else {
            transport_source.stop();
        }
    }
}

void Transport::changeListenerCallback(ChangeBroadcaster *source) {
    if (source == &transport_source) {
        play_button.setToggleState(transport_source.isPlaying(),
                                   dontSendNotification);
        if (transport_source.hasStreamFinished()) {
            transport_source.setPosition(0);
        }
    }
}