#include <cstdint>
#include <cmath>
#include <fstream>
#include <iostream>
#include <cassert>
#include "wave-generator.h"

WaveGenerator::WaveGenerator(uint16_t sampleRate, uint8_t sampleWidth, float volume) {
    assert(sampleWidth == 16 || sampleWidth == 32);

    this->sampleWidth = sampleWidth;
    this->sampleRate = sampleRate;

    // first we need to generate a sine wave LUT:
    this->max = 32767 * volume;
    if (sampleWidth == 32) {
        if (volume > 0.99) {
            volume = 0.99;
        }
        this->max = 2147483647 * volume;
    }

    this->min = -this->max;

    for (int i = 0; i < LUT_SIZE; ++i) {
        float coef = sinf(2 * M_PI * (float) i / LUT_SIZE);
        if (coef > 0.99) {
            coef = 0.99;
        }
        LUT[i] = (int32_t) (this->max * coef) * volume;
    }
}

uint32_t WaveGenerator::ms32size(uint32_t ms) {
    if (this->sampleWidth == 32) {
        return (this->sampleRate / 1000) * ms * 2;
    } else {
        return (this->sampleRate / 1000) * ms;
    }
}

void WaveGenerator::sine(uint32_t *buffer, uint32_t size, uint16_t frequency) {
    // NRF52 I2S should consist of L+R channel, int16/int32(?) encoded
    const float delta_phi = (float) frequency / this->sampleRate * LUT_SIZE / 2;

    if (this->sampleWidth == 32) size /= 2;
    for (int i = 0; i < size; ++i) {
        int phase_i = (int) this->phase; // get integer part of our phase

        if (this->sampleWidth == 32) {
            ((int32_t *) buffer)[2 * i] = ((int32_t *) buffer)[2 * i + 1] = this->LUT[phase_i];
        } else if (this->sampleWidth == 16) {
            ((int16_t *) buffer)[2 * i] = ((int16_t *) buffer)[2 * i + 1] = (short) this->LUT[phase_i];
        }

        this->phase += delta_phi; // increment phase
        if (this->phase >= (float) LUT_SIZE) // handle wraparound
            this->phase -= (float) LUT_SIZE;
    }
}

void WaveGenerator::square(uint32_t *buffer, uint32_t size, uint16_t frequency) {
    // NRF52 I2S should consist of L+R channel, int16(?) encoded
    const float delta_phi = (float) frequency / this->sampleRate * LUT_SIZE;

    int32_t threshold = this->max / 10 * 8;
    int32_t value = 0;

    // generate buffer of output
    if (this->sampleWidth == 32) size /= 2;
    for (int i = 0; i < size; ++i) {
        int phase_i = (int) this->phase; // get integer part of our phase

        // I know know, its just fastest solution
        if (this->LUT[phase_i] > threshold) {
            value = this->max;
        } else if (this->LUT[phase_i] < -threshold) {
            value = this->min;
        } else {
            value = this->LUT[phase_i];
        }

        if (this->sampleWidth == 32) {
            ((int32_t *) buffer)[2 * i] = ((int32_t *) buffer)[2 * i + 1] = value;
        } else if (this->sampleWidth == 16) {
            ((int16_t *) buffer)[2 * i] = ((int16_t *) buffer)[2 * i + 1] = (int16_t) value;
        }

        //printf("0x%" PRIXPTR "\n", (uintptr_t)&data_buffer[buffer_index][0]);

        this->phase += delta_phi; // increment phase
        if (this->phase >= (float) LUT_SIZE) // handle wraparound
            this->phase -= (float) LUT_SIZE;
    }
}

void WaveGenerator::sineTransform(uint32_t *buffer, uint32_t size, uint16_t frequency_from, uint16_t frequency_to) {
    const float delta_phi = (float) frequency_from / this->sampleRate * LUT_SIZE;
    const float target_delta_phi = (float) frequency_to / this->sampleRate * LUT_SIZE;

    if (this->sampleWidth == 32) size /= 2;
    const float increment = (target_delta_phi - delta_phi) / size;

    // generate buffer of output
    for (int i = 0; i < size; ++i) {
        int phase_i = (int) this->phase;     // get integer part of our phase
        int32_t sample = this->LUT[phase_i];       // get sample value from LUT

        if (this->sampleWidth == 32) {
            ((int32_t *) buffer)[2 * i] = ((int32_t *) buffer)[2 * i + 1] = sample;
        } else if (this->sampleWidth == 16) {
            ((int16_t *) buffer)[2 * i] = ((int16_t *) buffer)[2 * i + 1] = sample;
        }

        this->phase += delta_phi + increment * i;           // increment phase
        if (this->phase >= (float) LUT_SIZE) // handle wraparound
            this->phase -= (float) LUT_SIZE;

        //printf("%d\t%d\n", i, buffer[i]);
    }
}

void WaveGenerator::adjustVolume(uint32_t *buffer, uint32_t size, uint8_t volume_from, uint8_t volume_to) {
    const float from = (float) volume_from / 256;
    const float to = (float) volume_to / 256;

    if (this->sampleWidth == 32) size /= 2;
    const float increment = (to - from) / (float) size;

    for (int i = 0; i < size; ++i) {
        if (this->sampleWidth == 32) {
            ((int32_t *) buffer)[2 * i] = ((int32_t *) buffer)[2 * i + 1] =
                    ((int32_t *) buffer)[2 * i] *
                    (from + i * increment);
        } else if (this->sampleWidth == 16) {
            ((int16_t *) buffer)[2 * i] =
                    ((int16_t *) buffer)[2 * i + 1] =
                            (int16_t) (
                                    (float) ((int16_t *) buffer)[2 * i] *
                                    (from + i * increment)
                            );
        }
    }
}

namespace little_endian_io {
    template<typename Word>
    std::ostream &write_word(std::ostream &outs, Word value, unsigned size = sizeof(Word)) {
        for (; size; --size, value >>= 8)
            outs.put(static_cast <char> (value & 0xFF));
        return outs;
    }
}
using namespace little_endian_io;

void WaveGenerator::writeWav(uint32_t *buffer, uint32_t size, char *path) {
    std::ofstream f(path);

    // Write the file headers
    f << "RIFF----WAVEfmt ";     // (chunk size to be filled in later)
    write_word(f, 16, 4);  // no extension data
    write_word(f, 1, 2);  // PCM - integer samples
    write_word(f, 2, 2);  // two channels (stereo file)
    write_word(f, this->sampleRate, 4);  // samples per second (Hz)
    write_word(f, (this->sampleRate * this->sampleWidth * 2) / 8, 4);  // (Sample Rate * BitsPerSample * Channels) / 8
    write_word(f, (this->sampleWidth / 8) * 2,
               2);  // data block size (size of two integer samples, one for each channel, in bytes)
    write_word(f, this->sampleWidth, 2);  // number of bits per sample (use a multiple of 8)

    // Write the data chunk header
    size_t data_chunk_pos = f.tellp();
    f << "data----";  // (chunk size to be filled in later)

    for (int n = 0; n < size; n++) {
        write_word(f, buffer[n], 4);
    }

    // (We'll need the final file size to fix the chunk sizes above)
    size_t file_length = f.tellp();

    // Fix the data chunk header to contain the data size
    f.seekp(data_chunk_pos + 4);
    write_word(f, file_length - data_chunk_pos + 8);

    // Fix the file header to contain the proper RIFF chunk size, which is (file size - 8) bytes
    f.seekp(0 + 4);
    write_word(f, file_length - 8, 4);
}