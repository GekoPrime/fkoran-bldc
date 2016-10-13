#include <stdint.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include <avr/cpufunc.h>

#include "hal.h"
#include "lut.h"

#define PWM_PERIOD 600
#define THROTTLE_MAX ( (PWM_PERIOD-TOV_ISR_LEN)/256 )
#define PWM_ISR_LEN 40
#define TOV_ISR_LEN 40

#define BRIDGE_A 0
#define BRIDGE_B 1
#define BRIDGE_C 2 // could save a few instructions in HAL_PWM_Y_VECTOR if this were zero

uint16_t speed;
uint16_t phase;
uint8_t throttle;

register uint8_t event_x __asm("r4");
register uint8_t event_y __asm("r5");
register uint8_t bridge_x __asm("r6");
register uint8_t bridge_y __asm("r7");

register uint8_t bridge_x_buffer __asm("r8");
register uint8_t bridge_y_buffer __asm("r9");

volatile register uint8_t ready_for_update __asm("r10");

ISR(HAL_PWM_OVF_VECTOR)
{
    // OCRn is buffered in hardware
    // hal_n_set_ocr will take effect here
    event_x = 0;
    event_y = 0;
    bridge_x = bridge_x_buffer;
    bridge_y = bridge_y_buffer;
    ready_for_update = 1;
}

ISR(HAL_PWM_X_VECTOR)
{
    if (event_x == 0)
    {
        event_x++;
        if (bridge_x == BRIDGE_A)
        {
            hal_a_high();
        }
        else
        {
            hal_b_high();
        }
    }
    else
    {
        if (bridge_x == BRIDGE_A)
        {
            hal_a_low();
        }
        else
        {
            hal_b_low();
        }
    }
}

ISR(HAL_PWM_Y_VECTOR)
{
    if (event_y == 0)
    {
        event_y++;
        if (bridge_y == BRIDGE_C)
        {
            hal_c_high();
        }
        else
        {
            hal_b_high();
        }
    }
    else
    {
        if (bridge_y == BRIDGE_C)
        {
            hal_c_low();
        }
        else
        {
            hal_b_low();
        }
    }
}

void pwm_update()
{
    uint8_t i;
    uint8_t duty8[3];
    uint16_t duty[3];
    uint16_t x_duty;
    uint16_t y_duty;
    uint8_t x_bridge;
    uint8_t y_bridge;
    
    phase += speed;
    lut_interpolate(duty8, phase);

    // apply throttle multiplier and clamp duty[] to the appropriate range
    for (i=0; i<3; i++)
    {
        duty[i] = throttle * duty8[i];
        if (duty[i] < PWM_ISR_LEN)
        {
            duty[i] = 0;
        }
        if (duty[i] > PWM_PERIOD-TOV_ISR_LEN)
        {
            duty[i] = PWM_PERIOD-TOV_ISR_LEN;
        }
    }
    
    // default x and y to disabled
    x_duty = PWM_PERIOD+1;
    y_duty = PWM_PERIOD+1;
    x_bridge = BRIDGE_A;
    y_bridge = BRIDGE_C;
    
    // assign bridges to pwm channels
    if (duty[BRIDGE_A] != 0)
    {
        x_bridge = BRIDGE_A;
        x_duty = duty[BRIDGE_A];
    }
    else if (duty[BRIDGE_B] != 0)
    {
        x_bridge = BRIDGE_B;
        x_duty = duty[BRIDGE_B];
    }
    
    if (duty[BRIDGE_C] != 0)
    {
        y_bridge = BRIDGE_C;
        y_duty = duty[BRIDGE_C];
    }
    else if (x_bridge != BRIDGE_B && duty[BRIDGE_B] != 0)
    {
        y_bridge = BRIDGE_B;
        y_duty = duty[BRIDGE_B];
    }
    
    // atomic write
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        bridge_x_buffer = x_bridge;
        bridge_y_buffer = y_bridge;
        hal_x_set_ocr(PWM_PERIOD-x_duty);
        hal_y_set_ocr(PWM_PERIOD-y_duty);
    }
}

void hal_gpio_setup()
{
    HAL_TRACE_DDR  |=  HAL_TRACE_PIN;
    
    HAL_An_DDR  |=  HAL_An_PIN;
    HAL_Ap_DDR  |=  HAL_Ap_PIN;
    HAL_Bn_DDR  |=  HAL_Bn_PIN;
    HAL_Bp_DDR  |=  HAL_Bp_PIN;
    HAL_Cn_DDR  |=  HAL_Cn_PIN;
    HAL_Cp_DDR  |=  HAL_Cp_PIN;
    
    hal_a_low();
    hal_b_low();
    hal_c_low();
}

void setup()
{
    speed = 1000;//0x10000/60;
    phase = 0;
    throttle = 1;//THROTTLE_MAX;
    
    pwm_update();

    hal_gpio_setup();
    hal_pwm_timer_setup(PWM_PERIOD);
    
    sei();
}

int main()
{
    setup();  
    while (1)
    {
        if (ready_for_update)
        {
            pwm_update();
            ready_for_update = 0;
            continue;
        }
        hal_toggle_pin_atomic(&HAL_TRACE_PORT, HAL_TRACE_PIN);
    }
    return 0;
}

