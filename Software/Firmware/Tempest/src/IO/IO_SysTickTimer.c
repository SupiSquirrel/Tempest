/*

	Copyright (C) 2015 Matthias Friedrich

	This file is part of Tempest Firmware.

	Tempest Firmware is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Tempest Firmware is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with Tempest Firmware.  If not, see <http://www.gnu.org/licenses/>.
	
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

	SysTick_Config(system_gclk_gen_get_hz(GCLK_GENERATOR_0)/IO_SYSTICKTIMER_TICK_FREQUENCY);
	
}


uint32_t IO_SysTickTimer_GetTicks(void) {
	return IO_SysTickTimer_Context.Ticks;
}



uint32_t IO_SysTickTimer_GetCurrectMs(void) {
	return IO_SysTickTimer_Context.Ticks / (IO_SYSTICKTIMER_TICK_FREQUENCY / 1000);
}


void IO_SysTickTimer_WaitMs(uint32_t ms) {
	volatile uint32_t StartTicks;
	
	StartTicks = IO_SysTickTimer_Context.Ticks;
	
	do {} while (IO_SysTickTimer_Context.Ticks < StartTicks + (ms * (IO_SYSTICKTIMER_TICK_FREQUENCY / 1000)));
}


void SysTick_Handler(void) {
	IO_SysTickTimer_Context.Ticks++;
}