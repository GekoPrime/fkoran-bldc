#include "lut.h"

void lut_interpolate(uint8_t duty[], uint16_t phase)
{
    uint8_t index = (uint8_t)(phase >> 8);
    for (uint8_t i=0; i<3; i++)
    {
        duty[i] = lut[index][i];
    }
}
