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

    HAL_TIMSK1 |= HAL_PWM_X_EN;
    HAL_TIMSK1 |= HAL_PWM_Y_EN;
}

void hal_gpio_setup()
{

    hal_a_tristate();
    hal_b_tristate();
    hal_c_tristate();

    HAL_TRACE_DDR  |=  HAL_TRACE_PIN;

    HAL_SENSOR_DDR &= ~HAL_SENSOR_PIN;
    // may not need this. datasheet doesn't mention if output is bidirectional or open drain
    HAL_SENSOR_PORT |= HAL_SENSOR_PIN;

    HAL_An_DDR  |=  HAL_An_PIN;
    HAL_Ap_DDR  |=  HAL_Ap_PIN;
    HAL_Bn_DDR  |=  HAL_Bn_PIN;
    HAL_Bp_DDR  |=  HAL_Bp_PIN;
    HAL_Cn_DDR  |=  HAL_Cn_PIN;
    HAL_Cp_DDR  |=  HAL_Cp_PIN;
}


uint8_t hal_sensor_tick()
{
    // todo: there's a better way to do this
    static uint8_t previous_value;
    uint8_t new_value = hal_sensor_value();
    if (previous_value != new_value)
    {
        previous_value = new value;
        return 1;
    }
    else 
    {
        previous_value = new value;
        return 0;
    }
}
