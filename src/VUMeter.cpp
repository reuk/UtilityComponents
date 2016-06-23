#include "UtilityComponents/VUMeter.hpp"

void BufferReader::push_buffer(const AudioSourceChannelInfo &buffer) {
    push_buffer(*buffer.buffer);
}
void BufferReader::push_buffer(const AudioSampleBuffer &buffer) {
    push_buffer(buffer.getArrayOfReadPointers(),
                buffer.getNumChannels(),
                buffer.getNumSamples());
}

//----------------------------------------------------------------------------//

void Meter::do_push_buffer(const float **channel_data,
                           int num_channels,
                           int num_samples) {
    target = channel < num_channels
                     ? (*strategy)(channel_data[channel], num_samples)
                     : 0;
    actual = std::max(target, actual);
    actual += (target - actual) * 0.1;
}

void Meter::reset() {
    target = actual = 0;
}

float Meter::get_level() const {
    return actual;
};

//----------------------------------------------------------------------------//

VUMeter::VUMeter(size_t channel)
        : abs_meter(
                  [](const float *channel_data, int num_samples) {
                      return std::accumulate(channel_data,
                                             channel_data + num_samples,
                                             0.0f,
                                             [](auto i, auto j) {
                                                 return std::max(i,
                                                                 std::abs(j));
                                             });
                  },
                  channel)
        , rms_meter(
                  [](const float *channel_data, int num_samples) {
                      return std::sqrt(
                              std::accumulate(channel_data,
                                              channel_data + num_samples,
                                              0.0f,
                                              [](auto i, auto j) {
                                                  return i + j * j;
                                              }) /
                              num_samples);
                  },
                  channel) {
    startTimerHz(60);
}

void VUMeter::paint(Graphics &g) {
    do_paint(g);
}

void VUMeter::do_push_buffer(const float **channel_data,
                             int num_channels,
                             int num_samples) {
    abs_meter.push_buffer(channel_data, num_channels, num_samples);
    rms_meter.push_buffer(channel_data, num_channels, num_samples);
}

void VUMeter::reset() {
    abs_meter.reset();
    rms_meter.reset();
}

void VUMeter::timerCallback() {
    repaint();
}

float VUMeter::get_abs_level() const {
    return abs_meter.get_level();
}

float VUMeter::get_rms_level() const {
    return rms_meter.get_level();
}
