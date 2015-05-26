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
#include "FM_DetermineRof.h"
#include "TRG_Trigger.h"
#include "IO_Discretes.h"
#include "IO_SysTickTimer.h"
#include "IO_SerialUart.h"
#include "GBL_Defines.h"
#include "stdio.h"


#define PULL_TIMESTAMP_TICKS_SIZE 16


typedef struct {
	
	// Method 1
	uint32_t PullTimestampTicks[PULL_TIMESTAMP_TICKS_SIZE];
	uint32_t PullTimestampTicksPointer;
	
	// Method 2 
	uint32_t LastPullTimestamp;
	
	// Common
	float Rof;
	uint32_t RofTicks;

} FM_DetermineRof_Context_Type;

static FM_DetermineRof_Context_Type FM_DetermineRof_Context;



void FM_DetermineRof_Initialize() {
	uint32_t Counter;
	
	for (Counter = 0; Counter < PULL_TIMESTAMP_TICKS_SIZE; Counter++) {
		FM_DetermineRof_Context.PullTimestampTicks[Counter] = 0;
	}
	
	FM_DetermineRof_Context.PullTimestampTicksPointer = 0;
	FM_DetermineRof_Context.RofTicks                  = 0;
	FM_DetermineRof_Context.Rof                       = 0.0;
	FM_DetermineRof_Context.LastPullTimestamp         = 0;
}


void FM_DetermineRof_Update() {
	
	// Method 1
	
	/*uint32_t Counter;
	uint32_t TickDelayAct;
	uint32_t TickDelayPrev;
	uint32_t TickDelaySum;
	
	int32_t  TickDelay;
	uint32_t ValidStartTick;
	
	uint32_t ValidMeasurements;

	float AverageTickDelay;

	
	if (TRG_Trigger_GetEdge() == TRG_TRIGGER_EDGE_PULL) {

		FM_DetermineRof_Context.PullTimestampTicks[FM_DetermineRof_Context.PullTimestampTicksPointer % PULL_TIMESTAMP_TICKS_SIZE] = TRG_Trigger_GetPullTimestampTicks();
		
		FM_DetermineRof_Context.PullTimestampTicksPointer ++;
	}
	
	
	TickDelaySum      = 0;
	ValidMeasurements = 0;
	
	for (Counter = 0; Counter < PULL_TIMESTAMP_TICKS_SIZE-1; Counter++) {
		TickDelayAct  = FM_DetermineRof_Context.PullTimestampTicks[(FM_DetermineRof_Context.PullTimestampTicksPointer - Counter) % PULL_TIMESTAMP_TICKS_SIZE];
		TickDelayPrev = FM_DetermineRof_Context.PullTimestampTicks[(FM_DetermineRof_Context.PullTimestampTicksPointer - Counter - 1) % PULL_TIMESTAMP_TICKS_SIZE];
		
		// only consider pulls that happened in the last sec
		ValidStartTick = IO_SysTickTimer_GetTicks() - IO_SYSTICKTIMER_TICKS_PER_SEC;
		
		if ((TickDelayAct > ValidStartTick) && (TickDelayPrev > ValidStartTick)) {
			TickDelay = TickDelayAct - TickDelayPrev;
			
			if (TickDelay < 0) {
				TickDelay = 0;
			}
			
			ValidMeasurements ++;
			TickDelaySum += TickDelay;
		}
		
	}
	
	if (ValidMeasurements > 0) {
		AverageTickDelay = (float)TickDelaySum / (float)ValidMeasurements;
		FM_DetermineRof_Context.Rof = 1.0 / (AverageTickDelay / (float)IO_SYSTICKTIMER_TICKS_PER_SEC);
	} else {
		FM_DetermineRof_Context.Rof = 0.0;
	}*/
	
	// Method 2	
	
	uint32_t PullTimestamp;
	uint32_t PullTimestampDelta;
	
	if (IO_SysTickTimer_GetTicks() > FM_DetermineRof_Context.LastPullTimestamp + IO_SYSTICKTIMER_TICKS_PER_SEC) {
		PullTimestampDelta = 0;
		FM_DetermineRof_Context.RofTicks = 0;
		FM_DetermineRof_Context.Rof      = 0.0;		
	}
	
	
	if (TRG_Trigger_GetEdge() == TRG_TRIGGER_EDGE_PULL) {
		PullTimestamp = IO_SysTickTimer_GetTicks();
		
		PullTimestampDelta = PullTimestamp - FM_DetermineRof_Context.LastPullTimestamp;
		
		if (PullTimestampDelta < IO_SYSTICKTIMER_TICKS_PER_SEC) {
			FM_DetermineRof_Context.RofTicks = PullTimestampDelta;
			FM_DetermineRof_Context.Rof      = (float)IO_SYSTICKTIMER_TICKS_PER_SEC / (float)PullTimestampDelta;
		} else {
			FM_DetermineRof_Context.RofTicks = 0;
			FM_DetermineRof_Context.Rof      = 0.0;
		}
		
		
		FM_DetermineRof_Context.LastPullTimestamp = PullTimestamp;
	}
}


float FM_DetermineRof_GetRof(void) {
	return FM_DetermineRof_Context.Rof;
}


float FM_DetermineRof_GetRofTicks(void) {
	return FM_DetermineRof_Context.RofTicks;
}