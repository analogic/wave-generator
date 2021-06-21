// Generated Super Mario coin sound effect

#include <cstdio>
#include <cstring>
#include <wave-generator.h>

#define SAMPLE_RATE 44100
#define SAMPLE_WIDTH 16

#define BUFF_SIZE SAMPLE_RATE // our buffer is int32, so 16bit*2
#define ZERO_TRANSITION 100

int main(int argc, char** argv)
{
    WaveGenerator wg(SAMPLE_RATE, SAMPLE_WIDTH, 1.0f);

    static uint32_t buff[BUFF_SIZE];                // output buffer
    memset(&buff[0], (uint32_t)0, BUFF_SIZE);  // we set midpoint 128 (silence) since 8bit PCM is encoded as unsigned

    wg.square(&buff[0], wg.ms32size(100), 988);
    wg.square(&buff[wg.ms32size(100)], wg.ms32size(800), 1319);

    wg.adjustVolume(&buff[0], ZERO_TRANSITION, 0, 255);
    wg.adjustVolume(&buff[BUFF_SIZE / 10], wg.ms32size(800), 255, 0);

    for(int i = 0; i < BUFF_SIZE; i++) {
        printf("%d\t%d\n", i, ((int16_t *)buff)[i * 2]);
    }

    if (argc > 1) {
        wg.writeWav(&buff[0], BUFF_SIZE, argv[1]);
    }
}