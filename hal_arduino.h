#ifndef HAL_ARDUINO_H
#define HAL_ARDUINO_H

#include <stdint.h>
#include <avr/io.h>
#include <avr/cpufunc.h>

#define HAL_INVERTn

// simonk firmware uses a dead time of 300ns
// 5cy / 16MHz = 312.5ns
// set or clear will account for the 5th cycle
#define HAL_DEAD_TIME    5

#define HAL_PWM_OVF_VECTOR TIMER1_OVF_vect

#define HAL_PWM_X_VECTOR TIMER1_COMPA_vect
#define HAL_PWM_X_MATCH  OCR1A
#define HAL_PWM_X_EN ( _BV(OCIE1A) )

#define HAL_PWM_Y_VECTOR TIMER1_COMPB_vect
#define HAL_PWM_Y_MATCH  OCR1B
#define HAL_PWM_Y_EN ( _BV(OCIE1B) )

#define HAL_TRACE_DDR  DDRB
#define HAL_TRACE_PORT PORTB
#define HAL_TRACE_PIN  _BV(5)

#define HAL_SENSOR_DDR  DDRD
#define HAL_SENSOR_PORT PORTD
#define HAL_SENSOR_PIN  _BV(0)

#define HAL_An_DDR  DDRB
#define HAL_An_PORT PORTB
#define HAL_An_PIN  _BV(4)

#define HAL_Ap_DDR  DDRB
#define HAL_Ap_PORT PORTB
#define HAL_Ap_PIN  _BV(3)

#define HAL_Bn_DDR  DDRB
#define HAL_Bn_PORT PORTB
#define HAL_Bn_PIN  _BV(2)

#define HAL_Bp_DDR  DDRB
#define HAL_Bp_PORT PORTB
#define HAL_Bp_PIN  _BV(1)

#define HAL_Cn_DDR  DDRB
#define HAL_Cn_PORT PORTB
#define HAL_Cn_PIN  _BV(0)

#define HAL_Cp_DDR  DDRD
#define HAL_Cp_PORT PORTD
#define HAL_Cp_PIN  _BV(7)

#define HAL_TIMER1_INITA 0
#define HAL_TIMER1_INITB ( _BV(WGM13)|_BV(CS10) ) // WGM13 (dual slope PWM, update OCR and flag TOV on bottom, ICR is TOP), CS10 (1x prescale)
#define HAL_TIMSK1_INIT ( _BV(TOIE1) ) // TOV interrupt enable
#define HAL_TIMSK1_MASK ( _BV(TOIE1)|_BV(OCIE1A)|_BV(OCIE1B)|_BV(ICIE1) )
#define HAL_TIMSK1 TIMSK1
#define HAL_ICR1 ICR1
#define HAL_TCNT1 TCNT1
#define HAL_TCCR1A TCCR1A
#define HAL_TCCR1B TCCR1B

#endif
