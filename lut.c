#include "lut.h"

void lut_interpolate(uint8_t duty[], uint16_t phase)
{
    uint16_t index = ((uint32_t)phase * LUT_NUM_ENTRIES)/0x10000;
    for (uint8_t i=0; i<3; i++)
    {
        duty[i] = lut[index][i];
    }
}
