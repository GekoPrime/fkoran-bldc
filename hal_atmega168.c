#include "hal_atmega168.h"

void hal_pwm_timer_setup()
{
    TCCR1A = TIMER1_INITA;
    TCCR1B = TIMER1_INITB;
    TCNT1 = 0x0;
    TIMSK1 = TIMSK1_INIT;
    
    TCCR2A = TIMER2_INITA;
    TCCR2B = TIMER2_INITB;
    TCNT2 = 0x0;
    TIMSK2 = TIMSK2_INIT;
}

void hal_update_timer_setup()
{
    TCCR0A = TIMER0_INITA;
    TCCR0B = TIMER0_INITB;
    TCNT0 = 0xF;
    TIMSK0 = TIMSK0_INIT;
}
