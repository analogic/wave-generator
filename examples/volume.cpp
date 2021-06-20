// Click-free wave - smooth transition from/to zero with volume adjustment function

#include <cstdio>
#include <cstring>
#include <lut.h>

#define BUFF_SIZE 4096
#define TONE_A4 440
#define TONE_A6 1760

extern void write_as_wav(uint16_t *buffer, uint32_t size, char *path);

int main(int argc, char** argv)
{
    lut_pregenerate();
    static uint16_t buff[BUFF_SIZE];                // output buffer
    memset(&buff[0], (uint8_t)128, BUFF_SIZE * 2);  // we set midpoint 128 (silence) since 8bit PCM is encoded as unsigned

    lut_transform(&buff[0], BUFF_SIZE, TONE_A4, TONE_A6, 255);

    lut_adjust_volume(&buff[0], BUFF_SIZE / 8, 0, 255);
    lut_adjust_volume(&buff[BUFF_SIZE - BUFF_SIZE / 8], BUFF_SIZE / 8, 255, 0);

    for(int i = 0; i < BUFF_SIZE; i++) {
        printf("%d\t%d\n", i, ((uint8_t *)buff)[i * 2]);
    }

    if (argc > 1) {
        write_as_wav(&buff[0], BUFF_SIZE, argv[1]);
    }
}