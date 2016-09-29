#ifndef LUT_H
#define LUT_H

#include <stdint.h>
#include <string.h>

#include "hal.h"


// If OCR == TOP only a single interrupt will be generated in dual-slope modes
//
// An interrupt may be lost if one interrupt prevents another from clearing its flag before the latter reoccurs.
// Therefore interrupts must not repeat over a shorter period than the maximum blocking duration
// Therefore minimum pulse duration must be longer than the blocking duration
// This is enforced during LUT generation
// 96/HAL_PWM_PRESCALER seems to be a safe minimum for now. The PWM ISR is apx 64 cycles
//
// these #defines are ingested by the lookup table generator  
#define LUT_MAX_VAL 243
#define LUT_MIN_VAL 12
#define LUT_NUM_ENTRIES 256

extern uint8_t lut[][3];

void lut_interpolate(uint8_t duty[], uint16_t phase);

#endif
