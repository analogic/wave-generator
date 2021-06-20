#include <cstdint>
#include <climits>
#include <cmath>

#define LUT_SIZE 512 // lookup table size
#define SAMPLE_RATE 16666

static uint8_t LUT[LUT_SIZE];
float phase = 0.0f; // phase accumulator

void lut_pregenerate()
{
    // first we need to generate a sine wave LUT:
    for (int i = 0; i < LUT_SIZE; ++i)
    {
        LUT[i] = (uint8_t)roundf(INT8_MAX * (sinf(2.0f * M_PI * (float)i / LUT_SIZE) + (float)1));
    }
}

void lut_tone(uint16_t *buffer, uint16_t size, uint16_t frequency, uint8_t volume)
{
    // NRF52 I2S should consist of L+R channel, int16(?) encoded

    const float delta_phi = (float)frequency / SAMPLE_RATE * LUT_SIZE;
    const float volume_multiplicator = (float)volume / 255;

    // generate buffer of output
    for (int i = 0; i < size; ++i)
    {
        int phase_i = (int)phase ; // get integer part of our phase

        int8_t sample = LUT[phase_i] * volume_multiplicator; // get sample value from LUT

        ((uint8_t *)buffer)[2 * i] = sample;
        ((uint8_t *)buffer)[2 * i + 1] = 128;

        //printf("0x%" PRIXPTR "\n", (uintptr_t)&data_buffer[buffer_index][0]);

        phase += delta_phi; // increment phase
        if (phase >= (float)LUT_SIZE) // handle wraparound
            phase -= (float)LUT_SIZE;
    }
}

void lut_square_tone(uint16_t *buffer, uint16_t size, uint16_t frequency, uint8_t volume)
{
    // NRF52 I2S should consist of L+R channel, int16(?) encoded

    const float delta_phi = (float)frequency / SAMPLE_RATE * LUT_SIZE;
    const float volume_multiplicator = (float)volume / 255;

    const uint8_t max = INT8_MAX / 4;

    // generate buffer of output
    for (int i = 0; i < size; ++i)
    {
        int phase_i = (int)phase ; // get integer part of our phase

        uint8_t sample = LUT[phase_i] * volume_multiplicator; // get sample value from LUT

        // I know know, its just fastest solution
        if (sample > max + 128) {
            ((uint8_t *)buffer)[2 * i] = UINT8_MAX;
        } else if (sample < 128 - max) {
            ((uint8_t *)buffer)[2 * i] = 0;
        } else {
            ((uint8_t *)buffer)[2 * i] = (uint8_t)((sample - 128) * 4 + 128);
        }

        ((uint8_t *)buffer)[2 * i + 1] = 128;

        //printf("0x%" PRIXPTR "\n", (uintptr_t)&data_buffer[buffer_index][0]);

        phase += delta_phi; // increment phase
        if (phase >= (float)LUT_SIZE) // handle wraparound
            phase -= (float)LUT_SIZE;
    }
}

void lut_transform(uint16_t *buffer, uint16_t size, uint16_t frequency_from, uint16_t frequency_to, uint8_t volume) {
    const float delta_phi = (float)frequency_from / SAMPLE_RATE * LUT_SIZE;
    const float target_delta_phi = (float)frequency_to / SAMPLE_RATE * LUT_SIZE;

    const float increment = (target_delta_phi - delta_phi) / size;
    const float volume_multiplicator = (float)volume / 255;

    // generate buffer of output
    for (int i = 0; i < size; ++i)
    {
        int phase_i = (int)phase ;     // get integer part of our phase
        uint8_t sample = (LUT[phase_i] - 128) * volume_multiplicator;       // get sample value from LUT

        ((uint8_t *)buffer)[2 * i] = sample + 128;
        ((uint8_t *)buffer)[2 * i + 1] = 128;

        phase += delta_phi + increment * i;           // increment phase
        if (phase >= (float)LUT_SIZE) // handle wraparound
            phase -= (float)LUT_SIZE;

        //printf("%d\t%d\n", i, buffer[i]);
    }
}

void lut_adjust_volume(uint16_t *buffer, uint16_t size, uint8_t volume_from, uint8_t volume_to)
{
    const float from = (float)volume_from / 256;
    const float to = (float)volume_to / 256;

    const float increment = (to - from) / (float)size;

    for (int i = 0; i < size; ++i)
    {
        //printf("%d => ", (int16_t)buffer[i]);
        ((uint8_t *)buffer)[2 * i] = (((uint8_t *)buffer)[2 * i] - 128) * (from  + i * increment) + 128;
        //((uint8_t *)buffer)[2 * i + 1] = 0;
        //printf("%d\n", (int16_t)buffer[i]);
    }
}