#ifndef LUT_H
#define LUT_H

#include <stdint.h>
#include <string.h>

#include "hal.h"

#define LUT_NUM_ENTRIES 96

extern uint8_t lut[LUT_NUM_ENTRIES][3];

void lut_interpolate(uint8_t duty[], uint16_t phase);

#endif
