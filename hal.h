#ifndef HAL_H
#define HAL_H

#ifdef TARGET_HUGS
#include "hal_hugs.h"
#endif

#ifdef TARGET_ARDUINO
#include "hal_arduino.h"
#endif

#include <util/atomic.h>

void hal_pwm_timer_setup();
void hal_acomp_setup();
void hal_gpio_setup();

//
//
// the below must be included here so the compiler can inline
//
//

inline void hal_acomp_mux(uint8_t mux)
{
    ADMUX &= 0xFFFF;
    ADMUX |= mux;
}


inline uint8_t hal_acomp()
{
    return ACSR & _BV(ACO);
}

inline void hal_x_set_ocr(uint16_t duty)
{
    HAL_PWM_X_MATCH = duty;
}

inline void hal_y_set_ocr(uint16_t duty)
{
    HAL_PWM_Y_MATCH = duty;
}

inline void hal_dead_time()
{
    _NOP();
    _NOP();
    _NOP();
    _NOP();
    _NOP();
}

inline void hal_set_pin(volatile uint8_t* port, uint8_t mask)
{
    // this will compile to an atomic (sbi) if port and mask are constant
    *port |= mask;
}

inline void hal_clear_pin(volatile uint8_t* port, uint8_t mask)
{
    // this one too (cbi)
    *port &= ~mask;
}

inline void hal_toggle_pin(volatile uint8_t* port, uint8_t mask)
{
    // sorry, no atomic xor for io registers
    *port ^= mask;
}
inline void hal_toggle_pin_atomic(volatile uint8_t* port, uint8_t mask)
{
    // RMW is not interrupt-safe
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        hal_toggle_pin(port, mask);
    }
}

inline void hal_pwm_x_enable()
{
    HAL_TIMSK1 |= HAL_PWM_X_EN;
}

inline void hal_pwm_x_disable()
{
    HAL_TIMSK1 &= ~HAL_PWM_X_EN;
}

inline void hal_pwm_y_enable()
{
    HAL_TIMSK1 |= HAL_PWM_Y_EN;
}

inline void hal_pwm_y_disable()
{
    HAL_TIMSK1 &= ~HAL_PWM_Y_EN;
}

inline void hal_a_low()
{
    #ifdef HAL_INVERTp
    hal_set_pin(&HAL_Ap_PORT, HAL_Ap_PIN);
    #else
    hal_clear_pin(&HAL_Ap_PORT, HAL_Ap_PIN);
    #endif
    hal_dead_time();
    #ifdef HAL_INVERTn
    hal_clear_pin(&HAL_An_PORT, HAL_An_PIN);
    #else
    hal_set_pin(&HAL_An_PORT, HAL_An_PIN);
    #endif
}

inline void hal_a_high()
{
    #ifdef HAL_INVERTn
    hal_set_pin(&HAL_An_PORT, HAL_An_PIN);
    #else
    hal_clear_pin(&HAL_An_PORT, HAL_An_PIN);
    #endif
    hal_dead_time();
    #ifdef HAL_INVERTp
    hal_clear_pin(&HAL_Ap_PORT, HAL_Ap_PIN);
    #else
    hal_set_pin(&HAL_Ap_PORT, HAL_Ap_PIN);
    #endif
}

inline void hal_a_tristate()
{
    #ifdef HAL_INVERTn
    hal_set_pin(&HAL_An_PORT, HAL_An_PIN);
    #else
    hal_clear_pin(&HAL_An_PORT, HAL_An_PIN);
    #endif
    #ifdef HAL_INVERTp
    hal_set_pin(&HAL_Ap_PORT, HAL_Ap_PIN);
    #else
    hal_clear_pin(&HAL_Ap_PORT, HAL_Ap_PIN);
    #endif
}

inline void hal_b_low()
{
    #ifdef HAL_INVERTp
    hal_set_pin(&HAL_Bp_PORT, HAL_Bp_PIN);
    #else
    hal_clear_pin(&HAL_Bp_PORT, HAL_Bp_PIN);
    #endif
    hal_dead_time();
    #ifdef HAL_INVERTn
    hal_clear_pin(&HAL_Bn_PORT, HAL_Bn_PIN);
    #else
    hal_set_pin(&HAL_Bn_PORT, HAL_Bn_PIN);
    #endif
}

inline void hal_b_high()
{
    #ifdef HAL_INVERTn
    hal_set_pin(&HAL_Bn_PORT, HAL_Bn_PIN);
    #else
    hal_clear_pin(&HAL_Bn_PORT, HAL_Bn_PIN);
    #endif
    hal_dead_time();
    #ifdef HAL_INVERTp
    hal_clear_pin(&HAL_Bp_PORT, HAL_Bp_PIN);
    #else
    hal_set_pin(&HAL_Bp_PORT, HAL_Bp_PIN);
    #endif
}

inline void hal_b_tristate()
{
    #ifdef HAL_INVERTn
    hal_set_pin(&HAL_Bn_PORT, HAL_Bn_PIN);
    #else
    hal_clear_pin(&HAL_Bn_PORT, HAL_Bn_PIN);
    #endif
    #ifdef HAL_INVERTp
    hal_set_pin(&HAL_Bp_PORT, HAL_Bp_PIN);
    #else
    hal_clear_pin(&HAL_Bp_PORT, HAL_Bp_PIN);
    #endif
}

inline void hal_c_low()
{
    #ifdef HAL_INVERTp
    hal_set_pin(&HAL_Cp_PORT, HAL_Cp_PIN);
    #else
    hal_clear_pin(&HAL_Cp_PORT, HAL_Cp_PIN);
    #endif
    hal_dead_time();
    #ifdef HAL_INVERTn
    hal_clear_pin(&HAL_Cn_PORT, HAL_Cn_PIN);
    #else
    hal_set_pin(&HAL_Cn_PORT, HAL_Cn_PIN);
    #endif
}

inline void hal_c_high()
{
    #ifdef HAL_INVERTn
    hal_set_pin(&HAL_Cn_PORT, HAL_Cn_PIN);
    #else
    hal_clear_pin(&HAL_Cn_PORT, HAL_Cn_PIN);
    #endif
    hal_dead_time();
    #ifdef HAL_INVERTp
    hal_clear_pin(&HAL_Cp_PORT, HAL_Cp_PIN);
    #else
    hal_set_pin(&HAL_Cp_PORT, HAL_Cp_PIN);
    #endif
}

inline void hal_c_tristate()
{
    #ifdef HAL_INVERTn
    hal_set_pin(&HAL_Cn_PORT, HAL_Cn_PIN);
    #else
    hal_clear_pin(&HAL_Cn_PORT, HAL_Cn_PIN);
    #endif
    #ifdef HAL_INVERTp
    hal_set_pin(&HAL_Cp_PORT, HAL_Cp_PIN);
    #else
    hal_clear_pin(&HAL_Cp_PORT, HAL_Cp_PIN);
    #endif
}

#endif
