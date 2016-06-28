#pragma once

#include "juce_events/juce_events.h"
#include "juce_audio_devices/juce_audio_devices.h"

class PlaybackViewManager {
public:
    class Listener {
    public:
        Listener() = default;
        Listener(const Listener &) = default;
        Listener &operator=(const Listener &) = default;
        Listener(Listener &&) noexcept = default;
        Listener &operator=(Listener &&) noexcept = default;
        virtual ~Listener() noexcept = default;

        virtual void max_range_changed(PlaybackViewManager *r,
                                       const juce::Range<double> &range) = 0;
        virtual void visible_range_changed(PlaybackViewManager *r,
                                           const juce::Range<double> &range) = 0;
        virtual void current_time_changed(PlaybackViewManager *r,
                                          double time) = 0;
    };

    PlaybackViewManager() = default;
    virtual ~PlaybackViewManager() noexcept = default;

    void set_max_range(juce::Range<double> r, bool notify);
    juce::Range<double> get_max_range() const;
    void notify_max_range();

    void set_visible_range(juce::Range<double> r, bool notify);
    juce::Range<double> get_visible_range() const;
    void notify_visible_range();

    void set_follow_playback(bool b);
    bool get_follow_playback() const;

    void addListener(Listener *l);
    void removeListener(Listener *l);

protected:
    void set_current_time(double t);

private:
    juce::Range<double> max_range;
    juce::Range<double> visible_range;
    bool follow_playback{true};

    juce::ListenerList<Listener> listener_list;
};

//----------------------------------------------------------------------------//

class TransportViewManager : public PlaybackViewManager,
                             public juce::Timer,
                             public juce::ChangeListener {
public:
    TransportViewManager(juce::AudioTransportSource &audio_transport_source);
    virtual ~TransportViewManager() noexcept;

    void reset_view();

    void changeListenerCallback(juce::ChangeBroadcaster *cb) override;
    void timerCallback() override;

    void trigger();

    juce::AudioTransportSource &audio_transport_source;
};
