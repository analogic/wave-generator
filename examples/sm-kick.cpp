// Generated Super Mario kick sound effect

#include <stdio.h>
#include <lut.h>

#define BUFF_SIZE 33333 // same as SAMPLE_RATE

extern void write_as_wav(uint32_t *buffer, uint32_t size, char *path);

int main(int argc, char** argv)
{
    lut_pregenerate();
    static uint32_t buff[BUFF_SIZE];    // output buffer

    int slength = BUFF_SIZE/50; // 20ms
    int sdelay = slength * 4; // 80ms

    lut_square_tone(&buff[BUFF_SIZE / 10], slength, 500, 255);
    lut_square_tone(&buff[BUFF_SIZE / 10] + slength + sdelay, slength, 693, 255);

    for(int i = 0; i < BUFF_SIZE; i++) {
        printf("%d\t%d\n", i, ((int16_t *)buff)[i * 2]);
    }

    if (argc > 1) {
        write_as_wav(&buff[0], BUFF_SIZE, argv[1]);
    }
}