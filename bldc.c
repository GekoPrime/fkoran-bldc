#include <stdint.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include <avr/cpufunc.h>

#include "hal.h"
#include "lut.h"

uint8_t speed;
uint16_t phase;

// 0 means the next interrupt will occur as the counter value is increasing
//
// these variables would benefit from 'register ... __asm("rN") -ffixed-N'
// although there may currently be a bug in how register-fixed variables are treated by the gcc optimizer
//   http://www.avrfreaks.net/comment/1982591#comment-1982591
uint8_t timer_x_direction;
uint8_t timer_y_direction;
uint8_t timer_z_direction;

inline void set_pin(volatile uint8_t* port, uint8_t mask)
{
    // this will compile to an atomic (sbi) if port and mask are constant
    *port |= mask;
}

inline void clear_pin(volatile uint8_t* port, uint8_t mask)
{
    // this one too (cbi)
    *port &= ~mask;
}

inline void toggle_pin(volatile uint8_t* port, uint8_t mask)
{
    // sorry, no atomic xor for io registers
    *port ^= mask;
}

inline void toggle_pin_atomic(volatile uint8_t* port, uint8_t mask)
{
    // RMW is not interrupt-safe
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        toggle_pin(port, mask);
    }
}

inline void dead_time()
{
    // simonk firmware uses a dead time of 300ns
    // 5cy / 16MHz = 312.5ns
    // set or clear will account for the 5th cycle
    _NOP(); _NOP(); _NOP(); _NOP();
}

// match interrupt doesn't indicate whether it's a rising or descending slope
// we can track timer_x_direction,
// but there's a risk we'll miss an interrupt and get out of sync.
// therefore it's beneficial to restore timer_x_direction every time the counter overflows.
// inline assembly saves 14 cycles (push/pop r0,r1,sreg) and has a direct impact on the minimum duty cycle we can safely support
ISR(HAL_TIMER_XY_OVF_VECTOR, ISR_NAKED)
{
    __asm__("push r24");
    __asm__("ldi r24, 0");
    __asm__("sts timer_x_direction, r24"); // store the contects of r24 directly to timer_x_direction in data space
    __asm__("sts timer_y_direction, r24");
    __asm__("pop r24");
    __asm__("reti");
}

ISR(HAL_TIMER_Z_OVF_VECTOR, ISR_NAKED)
{
    __asm__("push r24");
    __asm__("ldi r24, 0");
    __asm__("sts timer_z_direction, r24");
    __asm__("pop r24");
    __asm__("reti");
}

ISR(HAL_TIMER_X_VECTOR)
{
    if (timer_x_direction == 0)
    {
        timer_x_direction = 1;
        #ifdef HAL_INVERT_Xp
        set_pin(&HAL_Ap_PORT, HAL_Ap_PIN);
        #else
        clear_pin(&HAL_Ap_PORT, HAL_Ap_PIN);
        #endif
        dead_time();
        set_pin(&HAL_An_PORT, HAL_An_PIN);
    }
    else
    {
        timer_x_direction = 0;
        clear_pin(&HAL_An_PORT, HAL_An_PIN);
        dead_time();
        #ifdef HAL_INVERT_Xp
        clear_pin(&HAL_Ap_PORT, HAL_Ap_PIN);
        #else
        set_pin(&HAL_Ap_PORT, HAL_Ap_PIN);
        #endif
    }
}

ISR(HAL_TIMER_Y_VECTOR)
{
    if (timer_y_direction == 0)
    {
        timer_y_direction = 1;
        #ifdef HAL_INVERT_Xp
        set_pin(&HAL_Bp_PORT, HAL_Bp_PIN);
        #else
        clear_pin(&HAL_Bp_PORT, HAL_Bp_PIN);
        #endif
        dead_time();
        set_pin(&HAL_Bn_PORT, HAL_Bn_PIN);
    }
    else
    {
        timer_y_direction = 0;
        clear_pin(&HAL_Bn_PORT, HAL_Bn_PIN);
        dead_time();
        #ifdef HAL_INVERT_Xp
        clear_pin(&HAL_Bp_PORT, HAL_Bp_PIN);
        #else
        set_pin(&HAL_Bp_PORT, HAL_Bp_PIN);
        #endif
    }
}

ISR(HAL_TIMER_Z_VECTOR)
{
    if (timer_z_direction == 0)
    {
        timer_z_direction = 1;
        #ifdef HAL_INVERT_Xp
        set_pin(&HAL_Cp_PORT, HAL_Cp_PIN);
        #else
        clear_pin(&HAL_Cp_PORT, HAL_Cp_PIN);
        #endif
        dead_time();
        set_pin(&HAL_Cn_PORT, HAL_Cn_PIN);
    }
    else
    {
        timer_z_direction = 0;
        clear_pin(&HAL_Cn_PORT, HAL_Cn_PIN);
        dead_time();
        #ifdef HAL_INVERT_Xp
        clear_pin(&HAL_Cp_PORT, HAL_Cp_PIN);
        #else
        set_pin(&HAL_Cp_PORT, HAL_Cp_PIN);
        #endif
    }
}


ISR(HAL_TIMER_UPDATE_VECTOR, ISR_NOBLOCK)
{
    uint8_t duty[3];
    phase += speed;
    lut_interpolate(duty, phase);
    
    HAL_TIMER_X_MATCH = duty[0];
    HAL_TIMER_Y_MATCH = duty[1];
    HAL_TIMER_Z_MATCH = duty[2];
}

void gpio_setup()
{
        HAL_TRACE_DDR  |=  HAL_TRACE_PIN;
        HAL_TRACE_PORT &= ~HAL_TRACE_PIN;
        
        HAL_An_DDR  |=  HAL_An_PIN;
        HAL_Ap_DDR  |=  HAL_Ap_PIN;
        HAL_Bn_DDR  |=  HAL_Bn_PIN;
        HAL_Bp_DDR  |=  HAL_Bp_PIN;
        HAL_Cn_DDR  |=  HAL_Cn_PIN;
        HAL_Cp_DDR  |=  HAL_Cp_PIN;
        
}

void setup()
{
    gpio_setup();

    hal_pwm_timer_setup();
    hal_update_timer_setup();

    speed = 100;
    phase = 0;

    sei();
}

int main()
{
    setup();  
    while (1)
    {
        // twiddle a pin so we can see when we're in an ISR
        set_pin(&HAL_TRACE_PORT, HAL_TRACE_PIN);
        _NOP(); _NOP(); // 2 nops to balance out the rjmp
        clear_pin(&HAL_TRACE_PORT, HAL_TRACE_PIN);
    }
    return 0;
}

