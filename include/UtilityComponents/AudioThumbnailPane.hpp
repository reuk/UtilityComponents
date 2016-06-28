#pragma once

#include "PlaybackViewManager.hpp"
#include "Playhead.hpp"

#include "juce_audio_utils/juce_audio_utils.h"

class AudioThumbnailPane : public juce::Component,
                           public juce::ChangeListener,
                           public PlaybackViewManager::Listener,
                           public Playhead::Listener {
public:
    AudioThumbnailPane(juce::AudioFormatManager& audio_format_manager,
                       TransportViewManager& transport_view_manager);
    virtual ~AudioThumbnailPane() noexcept;

    void paint(juce::Graphics& g) override;
    void resized() override;

    void changeListenerCallback(juce::ChangeBroadcaster* cb) override;

    void set_reader(juce::AudioFormatReader* new_reader, juce::int64 hash);

    void max_range_changed(PlaybackViewManager* r,
                           const juce::Range<double>& range) override;
    void visible_range_changed(PlaybackViewManager* r,
                               const juce::Range<double>& range) override;
    void current_time_changed(PlaybackViewManager* r, double time) override;

    void playhead_dragged(Playhead* p, const juce::MouseEvent& e) override;

private:
    double time_to_x(double t) const;
    double x_to_time(double t) const;
    void position_playhead();

    TransportViewManager& transport_view_manager;

    juce::AudioThumbnailCache audio_thumbnail_cache;
    juce::AudioThumbnail audio_thumbnail;
    Playhead playhead;
};
