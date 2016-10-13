#include "hal.h"

void hal_gpio_setup()
{
    HAL_TRACE_DDR  |=  HAL_TRACE_PIN;


    // fet gates as output
    HAL_An_DDR  |=  HAL_An_PIN;
    HAL_Ap_DDR  |=  HAL_Ap_PIN;
    HAL_Bn_DDR  |=  HAL_Bn_PIN;
    HAL_Bp_DDR  |=  HAL_Bp_PIN;
    HAL_Cn_DDR  |=  HAL_Cn_PIN;
    HAL_Cp_DDR  |=  HAL_Cp_PIN;
    
    // tristate fets
    hal_a_tristate();
    hal_b_tristate();
    hal_c_tristate();
    
    // back emf sense as input
    HAL_Aemf_DDR  &=  ~HAL_Aemf_PIN;
    HAL_Bemf_DDR  &=  ~HAL_Bemf_PIN;
    HAL_Cemf_DDR  &=  ~HAL_Cemf_PIN;
    
    // no pullup
    HAL_Aemf_PORT &=  ~HAL_Aemf_PIN;
    HAL_Bemf_PORT &=  ~HAL_Bemf_PIN;
    HAL_Cemf_PORT &=  ~HAL_Cemf_PIN;
}

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
}

void hal_acomp_setup()
{
    SFIOR |= _BV(ACME);
    ADCSRA &= ~_BV(ADEN);
}

