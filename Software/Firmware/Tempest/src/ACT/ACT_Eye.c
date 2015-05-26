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
#include "TRG_Trigger.h"
#include "IO_Discretes.h"
#include "IO_SysTickTimer.h"
#include "ACT_Solenoid.h"
#include "CFG_Config.h"
#include "FM_DetermineRof.h"
#include "ACT_Led.h"
#include "ACT_Eye.h"
#include "IO_ExtInt.h"


#define ACT_EYE_SAMPLE_FREQUENCY_TICKS 4


typedef struct {
	bool BallDetected;
	uint32_t LastSampleTicks;
} ACT_Eye_Context_Type;

static ACT_Eye_Context_Type ACT_Eye_Context;



void ACT_Eye_Initialize(void) {
	
	// pwm for eye tx and interrupt for eye rx is initialized in io_pwm / io_extint
	ACT_Eye_Context.BallDetected    = false;
	ACT_Eye_Context.LastSampleTicks = 0;
}


void ACT_Eye_Update(void) {
	
	// Method 1: polling, busy wait
	/* IO_Discretes_SetOutputActive(IO_DISCRETES_CHANNEL_OBS_TX);
	
	volatile uint32_t Delay;
	
	for (Delay = 0; Delay < 15; ) {
		Delay ++;
	}
	
	if (IO_Discretes_GetInputIsSet(IO_DISCRETES_CHANNEL_OBS_RX)) {
		// set == no ball detected
		ACT_Eye_Context.BallDetected = false;
	} else {
		ACT_Eye_Context.BallDetected = true;
	}
	
	IO_Discretes_SetOutputInactive(IO_DISCRETES_CHANNEL_OBS_TX); */
	
	// Method 2: hardware generated pwm for eye tx, external interrupt for rx
	if (IO_SysTickTimer_GetTicks() >= ACT_Eye_Context.LastSampleTicks + ACT_EYE_SAMPLE_FREQUENCY_TICKS) {
		
		ACT_Eye_Context.LastSampleTicks = IO_SysTickTimer_GetTicks();
		
		if (IO_ExtInt_ChanDetected(IO_EXTINT_OBS_RX_EIC_LINE)) {
			ACT_Eye_Context.BallDetected = false;
			IO_ExtInt_ClearDetected(IO_EXTINT_OBS_RX_EIC_LINE);
		} else {
			ACT_Eye_Context.BallDetected = true;
		}
	}
}


bool ACT_Eye_GetBallDetected(void) {
	return ACT_Eye_Context.BallDetected;
}