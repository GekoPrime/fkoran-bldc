#include "hal_atmega8.h"

void hal_pwm_timer_setup()
{
    TCCR1A = TIMER1_INITA;
    TCCR1B = TIMER1_INITB;
    TCNT1 = 0x0;
    TIMSK &= ~TIMSK1_MASK;
    TIMSK |= TIMSK1_INIT;
    
    TCCR2 = TIMER2_INIT;
    TCNT2 = 0x0;
    TIMSK &= ~TIMSK2_MASK;
    TIMSK |= TIMSK2_INIT;
}

void hal_update_timer_setup()
{
    TCCR0 = TIMER0_INIT;
    TCNT0 = 0xF;
    TIMSK &= ~TIMSK0_MASK;
    TIMSK |= TIMSK0_INIT;
}
