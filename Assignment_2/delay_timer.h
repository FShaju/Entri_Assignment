/*
 * delay_timer.h
 *
 * Created: 2026-05-18 12:39:49 PM
 *  Author: fedbi
 */ 


#ifndef DELAY_TIMER_H_
#define DELAY_TIMER_H_

#include <stdint.h>

extern volatile uint16_t stay;

void delay_init (void);

#endif /* DELAY_TIMER_H_ */