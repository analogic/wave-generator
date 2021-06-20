// Generated Super Mario kick sound effect

#include <cstdio>
#include <cstring>
#include <lut.h>

#define BUFF_SIZE 16666 // same as sample rate

extern void write_as_wav(uint16_t *buffer, uint32_t size, char *path);

int main(int argc, char** argv)
{
    lut_pregenerate();
    static uint16_t buff[BUFF_SIZE];                // output buffer
    memset(&buff[0], (uint8_t)128, BUFF_SIZE * 2);  // we set midpoint 128 (silence) since 8bit PCM is encoded as unsigned

    int start = BUFF_SIZE/4; // 100ms
    int slength = BUFF_SIZE/50; // 20ms
    int sdelay = slength * 4; // 80ms

    lut_square_tone(&buff[start + BUFF_SIZE / 10], slength, 500, 255);
    lut_square_tone(&buff[start + BUFF_SIZE / 10] + slength + sdelay, slength, 693, 255);

    for(int i = 0; i < BUFF_SIZE; i++) {
        printf("%d\t%d\n", i, ((uint8_t *)buff)[i * 2]);
    }

    if (argc > 1) {
        write_as_wav(&buff[0], BUFF_SIZE, argv[1]);
    }
}