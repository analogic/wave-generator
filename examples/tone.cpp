// Simple tone generation, since lib save phase to the state internally there should be uninterrupted wave

#include <cstdio>
#include <cstring>
#include <lut.h>

#define BUFF_SIZE 4096
#define TONE_A 440

extern void write_as_wav(uint16_t *buffer, uint32_t size, char *path);

int main(int argc, char** argv)
{
    lut_pregenerate();
    static uint16_t buff[BUFF_SIZE];                // output buffer
    memset(&buff[0], (uint8_t)128, BUFF_SIZE * 2);  // we set midpoint 128 (silence) since 8bit PCM is encoded as unsigned

    lut_tone(&buff[0], 400, TONE_A, 255);
    lut_tone(&buff[400], BUFF_SIZE/4, TONE_A, 255);
    lut_tone(&buff[BUFF_SIZE/4 + 400], BUFF_SIZE/4, TONE_A, 255);
    lut_tone(&buff[BUFF_SIZE/4 * 2 + 400], BUFF_SIZE/4, TONE_A, 255);

    for(int i = 0; i < BUFF_SIZE; i++) {
        printf("%d\t%d\n", i, ((uint8_t *)buff)[i * 2]);
    }

    if (argc > 1) {
        write_as_wav(&buff[0], BUFF_SIZE, argv[1]);
    }
}