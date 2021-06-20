// Generated Super Mario coin sound effect

#include <cstdio>
#include <cstring>
#include <lut.h>

#define BUFF_SIZE 16666
#define ZERO_TRANSITION 100

extern void write_as_wav(uint16_t *buffer, uint32_t size, char *path);

int main(int argc, char** argv)
{
    lut_pregenerate();
    static uint16_t buff[BUFF_SIZE];                // output buffer
    memset(&buff[0], (uint8_t)128, BUFF_SIZE * 2);  // we set midpoint 128 (silence) since 8bit PCM is encoded as unsigned

    lut_square_tone(&buff[0], BUFF_SIZE / 10, 988, 255);
    lut_square_tone(&buff[BUFF_SIZE / 10], BUFF_SIZE / 10 * 7, 1319, 255);

    lut_adjust_volume(&buff[0], ZERO_TRANSITION, 0, 255);
    lut_adjust_volume(&buff[BUFF_SIZE / 10], BUFF_SIZE / 10 * 7, 255, 0);

    for(int i = 0; i < BUFF_SIZE; i++) {
        printf("%d\t%d\n", i, ((uint8_t *)buff)[i * 2]);
    }

    if (argc > 1) {
        write_as_wav(&buff[0], BUFF_SIZE, argv[1]);
    }
}