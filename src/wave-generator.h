#pragma once

#include <cstdio>
#include <cstdint>
#include <climits>
#include <cmath>

#define LUT_SIZE 512 // lookup table size

class WaveGenerator {
private:
    int32_t LUT[LUT_SIZE];
    int32_t max;
    int32_t min;
    float phase = 0.0f; // phase accumulator

public:
    float volume;
    uint16_t sampleRate;
    uint8_t sampleWidth;
    WaveGenerator(uint16_t sampleRate, uint8_t sampleWidth, float volume);
    uint32_t ms32size(uint32_t ms);
    void sine(uint32_t *buffer, uint32_t size, uint16_t frequency);
    void square(uint32_t *buffer, uint32_t size, uint16_t frequency);
    void sineTransform(uint32_t *buffer, uint32_t size, uint16_t frequency_from, uint16_t frequency_to);
    void adjustVolume(uint32_t *buffer, uint32_t size, float volume_from, float volume_to);
    void writeWav(uint32_t *buffer, uint32_t size, char *path);
};
