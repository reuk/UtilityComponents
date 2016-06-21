#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include <numeric>

class BufferReader {
public:
    virtual ~BufferReader() noexcept = default;

    void push_buffer(const AudioSourceChannelInfo &buffer);
    void push_buffer(const AudioSampleBuffer &buffer);
    virtual void push_buffer(const float **channel_data,
                             int num_channels,
                             int num_samples) = 0;
};

//----------------------------------------------------------------------------//

class Meter : public BufferReader {
public:
    template <typename T>
    Meter(T &&strategy, size_t channel)
            : strategy(std::make_unique<TemplateMeterStrategy<T>>(
                      std::forward<T>(strategy)))
            , channel(channel) {
    }

    void push_buffer(const float **channel_data,
                     int num_channels,
                     int num_samples) override;

    void reset();

    float get_level() const;

private:
    struct MeterStrategy {
        virtual ~MeterStrategy() noexcept = default;
        virtual float operator()(const float *channel_data,
                                 int num_samples) const = 0;
    };

    template <typename T>
    struct TemplateMeterStrategy : public MeterStrategy {
        TemplateMeterStrategy(T &&t)
                : t(std::forward<T>(t)) {
        }
        float operator()(const float *channel_data,
                         int num_samples) const override {
            return t(channel_data, num_samples);
        }

    private:
        T t;
    };

    std::unique_ptr<MeterStrategy> strategy;
    size_t channel;
    float target{0};
    float actual{0};
};

//----------------------------------------------------------------------------//

class VUMeter : public Component, public BufferReader, private Timer {
public:
    VUMeter(size_t channel);

    float get_abs_level() const;
    float get_rms_level() const;

    void paint(Graphics &g) override;

    void push_buffer(const float **channel_data,
                     int num_channels,
                     int num_samples) override;

    void reset();

private:
    virtual void do_paint(Graphics &g) = 0;

    void timerCallback() override;

    Meter abs_meter;
    Meter rms_meter;
};