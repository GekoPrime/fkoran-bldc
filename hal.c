#include "hal.h"

void hal_pwm_timer_setup(uint16_t top)
{
    HAL_TCCR1A = HAL_TIMER1_INITA;
    HAL_TCCR1B = HAL_TIMER1_INITB;
    HAL_TCNT1 = 0x0;
    HAL_TIMSK1 &= HAL_TIMSK1_MASK;
    HAL_TIMSK1 |= HAL_TIMSK1_INIT;
    HAL_ICR1 = top;
    
    hal_x_set_ocr(top+1);
    hal_y_set_ocr(top+1);
    hal_pwm_x_enable();
    hal_pwm_y_enable();
}
