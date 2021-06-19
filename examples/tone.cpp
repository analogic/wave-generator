// Simple tone generation, since lib save phase to the state internally there should be uninterrupted wave

#include <stdio.h>
#include <lut.h>

#define BUFF_SIZE 4096
#define TONE_A 440

extern void write_as_wav(uint32_t *buffer, uint32_t size, char *path);

int main(int argc, char** argv)
{
    lut_pregenerate();
    static uint32_t buff[BUFF_SIZE];    // output buffer

    lut_tone(&buff[0], (uint32_t)400, TONE_A, 255);
    lut_tone(&buff[400], BUFF_SIZE/4, TONE_A, 255);
    lut_tone(&buff[BUFF_SIZE/4 + 400], BUFF_SIZE/4, TONE_A, 255);
    lut_tone(&buff[BUFF_SIZE/4 * 2 + 400], BUFF_SIZE/4, TONE_A, 255);

    for(int i = 0; i < BUFF_SIZE; i++) {
        printf("%d\t%d\n", i, ((int16_t *)buff)[i * 2]);
    }

    if (argc > 1) {
        write_as_wav(&buff[0], BUFF_SIZE, argv[1]);
    }
}