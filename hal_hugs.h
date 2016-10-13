#ifndef HAL_HUGS_H
#define HAL_HUGS_H

#include <stdint.h>
#include <avr/io.h>
#include <avr/cpufunc.h>

// simonk firmware uses a dead time of 300ns
// 5cy / 16MHz = 312.5ns
// set or clear will account for the 5th cycle
#define HAL_DEAD_TIME    {_NOP(); _NOP(); _NOP(); _NOP();}

#define HAL_INVERTp

#define HAL_PWM_OVF_VECTOR TIMER1_OVF_vect

#define HAL_PWM_X_VECTOR TIMER1_COMPA_vect
#define HAL_PWM_X_MATCH  OCR1A
#define HAL_PWM_X_EN ( _BV(OCIE1A) )

#define HAL_PWM_Y_VECTOR TIMER1_COMPB_vect
#define HAL_PWM_Y_MATCH  OCR1B
#define HAL_PWM_Y_EN ( _BV(OCIE1B) )

#define HAL_TRACE_DDR  DDRB
#define HAL_TRACE_PORT PORTB
#define HAL_TRACE_PIN  _BV(3)

#define HAL_An_DDR  DDRD
#define HAL_An_PORT PORTD
#define HAL_An_PIN  _BV(5)

#define HAL_Ap_DDR  DDRD
#define HAL_Ap_PORT PORTD
#define HAL_Ap_PIN  _BV(4)

#define HAL_Aemf_DDR  DDRD
#define HAL_Aemf_PORT PORTD
#define HAL_Aemf_PIN  0 // NOP; this channel uses a dedicated PWM pin
#define HAL_Aemf_MUX  6

#define HAL_Bn_DDR  DDRC
#define HAL_Bn_PORT PORTC
#define HAL_Bn_PIN  _BV(4)

#define HAL_Bp_DDR  DDRC
#define HAL_Bp_PORT PORTC
#define HAL_Bp_PIN  _BV(5)

#define HAL_Bemf_DDR  DDRD
#define HAL_Bemf_PORT PORTD
#define HAL_Bemf_PIN  0 // NOP; this channel uses a dedicated PWM pin
#define HAL_Bemf_MUX  7

#define HAL_Cn_DDR  DDRB
#define HAL_Cn_PORT PORTB
#define HAL_Cn_PIN  _BV(0)

#define HAL_Cp_DDR  DDRC
#define HAL_Cp_PORT PORTC
#define HAL_Cp_PIN  _BV(3)

#define HAL_Cemf_DDR  DDRC
#define HAL_Cemf_PORT PORTC
#define HAL_Cemf_PIN  _BV(0)
#define HAL_Cemf_MUX  0

#define HAL_TIMER1_INITA 0
#define HAL_TIMER1_INITB ( _BV(WGM13)|_BV(CS10) ) // WGM13 (dual slope PWM, update OCR and flag TOV on bottom, ICR is TOP), CS10 (1x prescale)
#define HAL_TIMSK1_INIT ( _BV(TOIE1) ) // TOV interrupt enable
#define HAL_TIMSK1_MASK ( _BV(TOIE1)|_BV(OCIE1A)|_BV(OCIE1B)|_BV(TICIE1) )
#define HAL_TIMSK1 TIMSK
#define HAL_ICR1 ICR1
#define HAL_TCNT1 TCNT1
#define HAL_TCCR1A TCCR1A
#define HAL_TCCR1B TCCR1B


#endif
