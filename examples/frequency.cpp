// Smooth frequency change

#include <stdio.h>
#include <lut.h>

#define BUFF_SIZE 16384
#define TONE_A4 440
#define TONE_A6 1760

extern void write_as_wav(uint32_t *buffer, uint32_t size, char *path);

int main(int argc, char** argv)
{
    lut_pregenerate();
    static uint32_t buff[BUFF_SIZE];    // output buffer

    lut_transform(&buff[0], BUFF_SIZE, TONE_A4, TONE_A6, 255);

    for(int i = 0; i < BUFF_SIZE; i++) {
        printf("%d\t%d\n", i, ((int16_t *)buff)[i * 2]);
    }

    if (argc > 1) {
        write_as_wav(&buff[0], BUFF_SIZE, argv[1]);
    }
}