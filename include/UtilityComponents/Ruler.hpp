#pragma once

#include "PlaybackViewManager.hpp"
#include "Playhead.hpp"

class Ruler : public juce::Component, public PlaybackViewManager::Listener {
public:
    Ruler(PlaybackViewManager& m);
    virtual ~Ruler() noexcept;

    void paint(juce::Graphics& g) override;

    double x_to_time(double x) const;
    double time_to_x(double time) const;

    void max_range_changed(PlaybackViewManager* r,
                           const juce::Range<double>& range) override;
    void visible_range_changed(PlaybackViewManager* r,
                               const juce::Range<double>& range) override;
    void current_time_changed(PlaybackViewManager* r, double time) override;

    void mouseEnter(const juce::MouseEvent& event) override;
    void mouseExit(const juce::MouseEvent& event) override;
    void mouseDown(const juce::MouseEvent& event) override;
    void mouseUp(const juce::MouseEvent& event) override;
    void mouseDrag(const juce::MouseEvent& event) override;
    void mouseDoubleClick(const juce::MouseEvent& event) override;

private:
    PlaybackViewManager& playback_view_manager;

    struct RulerState;
    std::unique_ptr<RulerState> ruler_state;
};