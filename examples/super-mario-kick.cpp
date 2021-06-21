// Generated Super Mario kick sound effect

#include <cstdio>
#include <cstring>
#include <wave-generator.h>

#define SAMPLE_RATE 44100
#define SAMPLE_WIDTH 16

#define BUFF_SIZE SAMPLE_RATE

int main(int argc, char** argv)
{
    WaveGenerator wg(SAMPLE_RATE, SAMPLE_WIDTH, 1.0f);

    static uint32_t buff[BUFF_SIZE];                // output buffer
    memset(&buff[0], (uint32_t)0, BUFF_SIZE);  // we set midpoint 128 (silence) since 8bit PCM is encoded as unsigned

    int start = wg.ms32size(100);
    int slength = wg.ms32size(20); // 20ms
    int sdelay = wg.ms32size(80); // 80ms

    wg.square(&buff[start], slength, 500);
    wg.square(&buff[start + slength] + sdelay, slength, 693);

    for(int i = 0; i < BUFF_SIZE; i++) {
        printf("%d\t%d\n", i, ((int16_t *)buff)[i * 2]);
    }

    if (argc > 1) {
        wg.writeWav(&buff[0], BUFF_SIZE, argv[1]);
    }
}