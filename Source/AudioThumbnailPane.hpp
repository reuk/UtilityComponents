#pragma once

#include "PlaybackViewManager.hpp"
#include "Playhead.hpp"

class AudioThumbnailPane : public Component,
                           public ChangeListener,
                           public PlaybackViewManager::Listener,
                           public Playhead::Listener {
public:
    AudioThumbnailPane(AudioFormatManager& audio_format_manager,
                       TransportViewManager& transport_view_manager);
    virtual ~AudioThumbnailPane() noexcept;

    void paint(Graphics& g) override;
    void resized() override;

    void changeListenerCallback(ChangeBroadcaster* cb) override;

    void set_reader(AudioFormatReader* new_reader, int64 hash);

    void max_range_changed(PlaybackViewManager* r,
                           const Range<double>& range) override;
    void visible_range_changed(PlaybackViewManager* r,
                               const Range<double>& range) override;
    void current_time_changed(PlaybackViewManager* r, double time) override;

    void playhead_dragged(Playhead* p, const MouseEvent& e) override;

private:
    double time_to_x(double t) const;
    double x_to_time(double t) const;
    void position_playhead();

    TransportViewManager& transport_view_manager;

    AudioThumbnailCache audio_thumbnail_cache;
    AudioThumbnail audio_thumbnail;
    Playhead playhead;
};
