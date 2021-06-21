// Simple tone generation, since lib save phase to the state internally there should be uninterrupted wave

#include <cstdio>
#include <cstring>
#include <wave-generator.h>

#define SAMPLE_RATE 44100
#define SAMPLE_WIDTH 16

#define BUFF_SIZE SAMPLE_RATE // two channels
#define TONE_A 440

int main(int argc, char** argv)
{
    WaveGenerator wg(SAMPLE_RATE, SAMPLE_WIDTH, 1.0f);

    static uint32_t buff[BUFF_SIZE];
    memset(&buff[0], (uint16_t)0, BUFF_SIZE * 2);

    wg.sine(&buff[0], wg.ms32size(250), TONE_A);
    wg.sine(&buff[wg.ms32size(250)], wg.ms32size(250), TONE_A);
    wg.sine(&buff[wg.ms32size(500)], wg.ms32size(250), TONE_A);

    for(int i = 0; i < BUFF_SIZE; i++) {
        printf("%d\t%d\n", i, ((int16_t *)buff)[i * 2]);
    }

    if (argc > 1) {
        wg.writeWav(&buff[0], BUFF_SIZE, argv[1]);
    }
}