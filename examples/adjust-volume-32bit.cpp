// Click-free wave - smooth transition from/to zero with volume adjustment function, 32 bit sample width
// (Firefox can't play 32 bit WAVs)

#include <cstdio>
#include <cstring>
#include <wave-generator.h>

#define SAMPLE_RATE 48000
#define SAMPLE_WIDTH 32

#define BUFF_SIZE SAMPLE_RATE / 2
#define TONE_A4 440
#define TONE_A6 1760

int main(int argc, char** argv)
{
    WaveGenerator wg(SAMPLE_RATE, SAMPLE_WIDTH, 1.0f);

    static uint32_t buff[BUFF_SIZE];           // output buffer
    memset(&buff[0], (int32_t)0, BUFF_SIZE);  // we set midpoint 128 (silence) since 8bit PCM is encoded as unsigned

    wg.sineTransform(&buff[0], BUFF_SIZE, TONE_A4, TONE_A6);

    wg.adjustVolume(&buff[0], wg.ms32size(50), 0, 255);
    wg.adjustVolume(&buff[BUFF_SIZE - wg.ms32size(50)], wg.ms32size(50), 1.0f, 0.0f);

    for(int i = 0; i < BUFF_SIZE / 2; i++) {
        printf("%d\t%d\n", i, ((int32_t *)buff)[i * 2]);
    }

    if (argc > 1) {
        wg.writeWav(&buff[0], BUFF_SIZE, argv[1]);
    }
}