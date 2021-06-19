// Generated Super Mario coin sound effect

#include <stdio.h>
#include <lut.h>

#define BUFF_SIZE 33333 // same as SAMPLE_RATE
#define ZERO_TRANSITION 100

extern void write_as_wav(uint32_t *buffer, uint32_t size, char *path);

int main(int argc, char** argv)
{
    lut_pregenerate();
    static uint32_t buff[BUFF_SIZE];    // output buffer

    lut_square_tone(&buff[0], BUFF_SIZE / 10, 988, 255);
    lut_square_tone(&buff[BUFF_SIZE / 10], BUFF_SIZE / 10 * 7, 1319, 255);

    lut_adjust_volume(&buff[0], ZERO_TRANSITION, 0, 255);
    lut_adjust_volume(&buff[BUFF_SIZE / 10], BUFF_SIZE / 10 * 7, 255, 0);

    for(int i = 0; i < BUFF_SIZE; i++) {
        printf("%d\t%d\n", i, ((int16_t *)buff)[i * 2]);
    }

    if (argc > 1) {
        write_as_wav(&buff[0], BUFF_SIZE, argv[1]);
    }
}