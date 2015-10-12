/*
 * IO_SysTickTimer.h
 *
 * Created: 13.04.2015 21:22:48
 *  Author: Squirrel
 */ 


#ifndef IO_SYSTICKTIMER_H_
#define IO_SYSTICKTIMER_H_

#define IO_SYSTICKTIMER_TICK_FREQUENCY 10000

void IO_SysTickTimer_Initialize(void);
uint32_t IO_SysTickTimer_GetTicks(void);
uint32_t IO_SysTickTimer_GetCurrectMs(void);


#endif /* IO_SYSTICKTIMER_H_ */