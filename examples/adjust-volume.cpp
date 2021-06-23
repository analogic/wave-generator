// Click-free wave - smooth transition from/to zero with volume adjustment function

#include <cstdio>
#include <cstring>
#include <wave-generator.h>

#define SAMPLE_RATE 44100
#define SAMPLE_WIDTH 16

#define BUFF_SIZE SAMPLE_RATE / 4
#define TONE_A4 440
#define TONE_A6 1760

int main(int argc, char** argv)
{
    WaveGenerator wg(SAMPLE_RATE, SAMPLE_WIDTH, 1.0f);

    static uint32_t buff[BUFF_SIZE];                // output buffer
    memset(&buff[0], (uint32_t)0, BUFF_SIZE);  // we set midpoint 128 (silence) since 8bit PCM is encoded as unsigned

    wg.sineTransform(&buff[0], BUFF_SIZE, TONE_A4, TONE_A6);

    wg.adjustVolume(&buff[0], wg.ms32size(50), 0, 255);
    wg.adjustVolume(&buff[BUFF_SIZE - wg.ms32size(50)], wg.ms32size(50), 1.0f, 0.0f);

    for(int i = 0; i < BUFF_SIZE; i++) {
        printf("%d\t%d\n", i, ((int16_t *)buff)[i * 2]);
    }

    if (argc > 1) {
        wg.writeWav(&buff[0], BUFF_SIZE, argv[1]);
    }
}