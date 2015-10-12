/*
 * IO_SysTickTimer.c
 *
 * Created: 13.04.2015 21:23:11
 *  Author: Squirrel
 */ 

#include <asf.h>
#include "IO_SysTickTimer.h"

typedef struct {
	uint32_t Ticks;	
} IO_SysTickTimer_Context_Type;

// Is volatile because of tick interrupt
static volatile IO_SysTickTimer_Context_Type IO_SysTickTimer_Context;




void IO_SysTickTimer_Initialize(void) {
	
	IO_SysTickTimer_Context.Ticks = 0;
	
	if (SysTick_Config(sysclk_get_cpu_hz() / IO_SYSTICKTIMER_TICK_FREQUENCY)) {
		while (1);
	}
	
}


uint32_t IO_SysTickTimer_GetTicks(void) {
	return IO_SysTickTimer_Context.Ticks;
}



uint32_t IO_SysTickTimer_GetCurrectMs(void) {
	return IO_SysTickTimer_Context.Ticks / (IO_SYSTICKTIMER_TICK_FREQUENCY/1000);
}




/**
 *  \brief Handler for System Tick interrupt.
 *
 *  Process System Tick Event
 *  Increments the g_ul_ms_ticks counter.
 */
void SysTick_Handler(void) {
	IO_SysTickTimer_Context.Ticks++;
}