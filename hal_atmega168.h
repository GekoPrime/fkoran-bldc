#ifndef HAL_ATMEGA168_H
#define HAL_ATMEGA168_H

#include <stdint.h>
#include <avr/io.h>

#define HAL_PWM_PRESCALER  8

#define HAL_TIMER_XY_OVF_VECTOR TIMER1_OVF_vect

#define HAL_TIMER_X_VECTOR TIMER1_COMPA_vect
#define HAL_TIMER_X_MATCH  OCR1A
#define HAL_TIMER_X_COUNT  TCNT1

#define HAL_TIMER_Y_VECTOR TIMER1_COMPB_vect
#define HAL_TIMER_Y_MATCH  OCR1B
#define HAL_TIMER_Y_COUNT  TCNT1

#define HAL_TIMER_Z_OVF_VECTOR TIMER2_OVF_vect

#define HAL_TIMER_Z_VECTOR TIMER2_COMPA_vect
#define HAL_TIMER_Z_MATCH  OCR2A
#define HAL_TIMER_Z_COUNT  TCNT2

#define HAL_TIMER_UPDATE_VECTOR TIMER0_OVF_vect
#define HAL_TIMER_UPDATE_PRESCALER  64

#define HAL_TRACE_DDR  DDRB
#define HAL_TRACE_PORT PORTB
#define HAL_TRACE_PIN  _BV(5)

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

#define TIMER0_INITA ( 0 ) // normal mode (8 bit single slope)
#define TIMER0_INITB ( _BV(CS00)|_BV(CS01) ) // CS00|CS01 (64:1 clock prescaler)
#define TIMSK0_INIT ( _BV(TOIE0) ) // overflow interrupt enable

#define TIMER1_INITA ( _BV(WGM10) ) // WGM10 (8-bit phase correct PWM)
#define TIMER1_INITB ( _BV(CS11) ) // CS10 (1:1 clock prescaler)
#define TIMSK1_INIT ( _BV(OCIE1A)|_BV(OCIE1B)|_BV(TOIE1) ) // OCR1A match enable, OCR1B match enable, overflow enable

#define TIMER2_INITA ( _BV(WGM20) ) // WGM20 (8-bit phase correct PWM)
#define TIMER2_INITB ( _BV(CS21) ) // CS20 (1:1 clock prescaler), 
#define TIMSK2_INIT ( _BV(OCIE2A)|_BV(TOIE2) ) // OCR2A match enable, overflow enable

void hal_pwm_timer_setup();

void hal_update_timer_setup();

#endif
