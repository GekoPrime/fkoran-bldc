#include <stdint.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include <avr/cpufunc.h>

#include "hal.h"
#include "lut.h"

#define DEMAG_TIME
#define TIMING_ADVANCE       ( 14*(0x10000/360) )
#define PHASE_TARGET_OFFSET  ( 0x10000/6 )
#define SENSOR_INCREMENT     ( 0x10000 / (2048/2) )

#define ALIGNMENT_TIME  100  // ms

#define PWM_PERIOD 600
#define THROTTLE_MAX  ( (PWM_PERIOD-TOV_ISR_LEN)/256 )
#define PWM_ISR_LEN 40
#define TOV_ISR_LEN 40

#define BRIDGE_A 0
#define BRIDGE_B 1
#define BRIDGE_C 2 // could save a few instructions in HAL_PWM_Y_VECTOR if this were zero

int16_t speed;
uint16_t phase;
uint8_t throttle;

register uint8_t event_x __asm("r4"); // is this the first match event of the PWM cycle or the second?
register uint8_t event_y __asm("r5");
register uint8_t bridge_x __asm("r6"); // what bridge is associated with PWM X? A or B?
register uint8_t bridge_y __asm("r7"); // what bridge is associated with PWM Y? B or C?

register uint8_t bridge_x_buffer __asm("r8");
register uint8_t bridge_y_buffer __asm("r9");

volatile register uint8_t ready_for_update __asm("r10"); // pwm_update should compute PWM values for next cycle

ISR(HAL_PWM_OVF_VECTOR)
{
    // OCRn is buffered in hardware
    // hal_n_set_ocr will take effect here
    event_x = 0;
    event_y = 0;
    bridge_x = bridge_x_buffer;
    bridge_y = bridge_y_buffer;
    ready_for_update = 1;
    // need to tristate the inactive channel
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
    uint16_t targetPhase;

    phase += SENSOR_INCREMENT * hal_sensor_tick();

    //  based on direction, decide what phase angle we should be driving
    targetPhase = phase;
    if (speed > 0)
    {
        targetPhase += PHASE_TARGET_OFFSET;
    }
    else if (speed < 0)
    {
        targetPhase -= PHASE_TARGET_OFFSET;
    }

    // get duty cycle from the lookup table
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


void setup()
{
    // align the motor at phase zero
    speed = 0;
    phase = 0;
    throttle = THROTTLE_MAX;
    pwm_update();

    hal_gpio_setup();
    hal_pwm_timer_setup(PWM_PERIOD);
    
    sei();
    _delay_ms(ALIGNMENT_TIME);
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

