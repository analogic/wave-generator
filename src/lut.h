#include <stdio.h>
#include <stdint.h>
#include <limits.h>
#include <math.h>

void lut_pregenerate();
void lut_tone(uint32_t *buffer, uint16_t size, uint16_t frequency, uint8_t volume);
void lut_transform(uint32_t *buffer, uint16_t size, uint16_t frequency_from, uint16_t frequency_to, uint8_t volume);
void lut_adjust_volume(uint32_t *buffer, uint16_t size, uint8_t volume_from, uint8_t volume_to);
void lut_square_tone(uint32_t *buffer, uint16_t size, uint16_t frequency, uint8_t volume);