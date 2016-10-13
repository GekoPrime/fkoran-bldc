#include <stdint.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include <avr/cpufunc.h>

#include "hal.h"
#include "lut.h"

#define PWM_PERIOD  ( 1200 )
#define THROTTLE    ( PWM_PERIOD/8)
#define ALIGNMENT_DURATION  ( 10 * (F_CPU/1000) ) // ms
#define ZERO_THRESHOLD

uint8_t phase;    // from 0 to 5
uint16_t time;    // incremented on every PWM tick
uint16_t time_z0; // time of most recent zero crossing
uint16_t time_z1; // time of second most recent zero crossing
uint16_t time_commutation; // time of next scheduled commutation

// suppress back-emf scheduled commutation during startup
uint8_t suppress_commutation;

uint8_t pwm_event; // idnetify slope when doing dual slope PWM

typedef enum {A, B, C} channel_t;
channel_t role_vcc[6] = {A, A, B, B, C, C};
channel_t role_gnd[6] = {B, C, C, A, A, B};
channel_t role_tri[6] = {C, B, A, C, B, A};
// analog comparator takes reference as +
//                               mux as -
// if waiting for a rising edge, wait for ACO == 0
// OCR ISR uses XOR to avoid an awkward _BV()
// so polarity is inverted. Rising edge is slope 1
uint8_t  emf_slope[6] = {0, 1, 0, 1, 0, 1};

uint8_t pwm_channel;

ISR(HAL_PWM_OVF_VECTOR)
{
    hal_toggle_pin_atomic(&HAL_TRACE_PORT, HAL_TRACE_PIN);

    time++;
    
    if (time == time_commutation && !suppress_commutation)
    {
        phase = (phase+1)%6;
    }

    if (hal_acomp())
    {
    //    hal_set_pin(&HAL_TRACE_PORT, HAL_TRACE_PIN);
    }
    else
    {
    //    hal_clear_pin(&HAL_TRACE_PORT, HAL_TRACE_PIN);
    }
        hal_a_low();
        hal_b_low();
        hal_c_low();

    if (hal_acomp() ^ emf_slope[phase])
    {
        time_commutation = time + (time_z0-time_z1)/2;
        time_z1 = time_z0;
        time_z0 = time;
    }
    
    if (role_tri[phase] == A)
    {
        hal_acomp_mux(HAL_Aemf_MUX);
        hal_a_tristate();
        hal_b_low();
        hal_c_low();
    }
    else if (role_tri[phase] == B)
    {
        hal_acomp_mux(HAL_Bemf_MUX);
        hal_a_low();
        hal_b_tristate();
        hal_c_low();
    }
    else if (role_tri[phase] == C)
    {
        hal_acomp_mux(HAL_Cemf_MUX);
        hal_a_low();
        hal_b_low();
        hal_c_tristate();
    }

    HAL_PWM_X_MATCH = PWM_PERIOD-THROTTLE;
    pwm_event = 0;
    pwm_channel = role_vcc[phase];
}

ISR(HAL_PWM_X_VECTOR)
{
    if (pwm_event == 0)
    {

    if (pwm_channel == A)
    {
        hal_a_high();
    }
    else if (pwm_channel == B)
    {
        hal_b_high();
    }
    else if (pwm_channel == C)
    {
        hal_c_high();
    }

    }
    else
    {

    if (pwm_channel == A)
    {
        hal_a_low();
    }
    else if (pwm_channel == B)
    {
        hal_b_low();
    }
    else if (pwm_channel == C)
    {
        hal_c_low();
    }

    }

    pwm_event++;
}

    
void open_loop_startup()
{
    suppress_commutation = 1;
    
    _delay_ms(80);
    phase = (phase+1)%6;
    _delay_ms(40);
    phase = (phase+1)%6;
    _delay_ms(20);
    phase = (phase+1)%6;

    //suppress_commutation = 0;
}

void setup()
{
    hal_gpio_setup();
    hal_pwm_timer_setup(PWM_PERIOD);
    hal_acomp_setup();
    
    phase = 0;
    time = 0;
    time_commutation = 0xFFFF;

    sei();

    open_loop_startup();
    suppress_commutation = 1;
}

int main()
{
    setup();  
    while (1)
    {
    //hal_toggle_pin_atomic(&HAL_TRACE_PORT, HAL_TRACE_PIN);
    }
    return 0;
}

