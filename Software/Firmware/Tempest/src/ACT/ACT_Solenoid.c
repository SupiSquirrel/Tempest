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
#include "IO_Pwm.h"
#include "ACT_Eye.h"


typedef struct {
	uint32_t SolenoidActiveStartTick;
	bool SolenoidActive;
	uint32_t ShotCount;
	
	uint32_t EyeMode;
} ACT_Solenoid_Context_Type;

static ACT_Solenoid_Context_Type ACT_Solenoid_Context;



void ACT_Solenoid_Initialize(void) {
	IO_Discretes_SetOutputInactive(IO_DISCRETES_CHANNEL_SOLENOID);
	ACT_Solenoid_Context.SolenoidActive          = false;
	ACT_Solenoid_Context.SolenoidActiveStartTick = 0;
	ACT_Solenoid_Context.ShotCount               = CFG_Config_GetShotCount();
	
	ACT_Solenoid_Context.EyeMode                 = CFG_Config_GetUserSettingEyesOn();
}


void ACT_Solenoid_Update(void) {
	
	// Method 1: manual
	/*if (ACT_Solenoid_Context.SolenoidActive) {
		if (IO_SysTickTimer_GetTicks() > ACT_Solenoid_Context.SolenoidActiveStartTick + CFG_Config_GetUserSettingDwellTicks()) {
			ACT_Solenoid_Context.SolenoidActive = false;
			IO_Discretes_SetOutputInactive(IO_DISCRETES_CHANNEL_SOLENOID);
		}
	}*/
	
}


void ACT_Solenoid_FireShot(void) {
	
	// test
	ACT_Solenoid_Context.EyeMode                 = CFG_Config_GetUserSettingEyesOn();
	
	// Method 1: manual
	/*if (!ACT_Solenoid_Context.SolenoidActive) {
		ACT_Solenoid_Context.SolenoidActive          = true;
		ACT_Solenoid_Context.SolenoidActiveStartTick = IO_SysTickTimer_GetTicks();
		
		// instant activation - not synced to tick edge
		// this leads to a jitter of 1/IO_SYSTICKTIMER_TICK_FREQUENCY [s]
		IO_Discretes_SetOutputActive(IO_DISCRETES_CHANNEL_SOLENOID);
		ACT_Solenoid_Context.ShotCount ++;
	}*/
	
	// Method 2: one-shot timer
	if (ACT_Solenoid_Context.EyeMode == ACT_SOENOID_EYE_MODE_ACTIVE) {
		if (ACT_Eye_GetBallDetected()) {
			ACT_Solenoid_Context.SolenoidActiveStartTick = IO_SysTickTimer_GetTicks();
			IO_Pwm_OneShotTcc1(CFG_Config_GetUserSettingDwellTicks() * (1000000 / IO_SYSTICKTIMER_TICKS_PER_SEC));
			ACT_Solenoid_Context.ShotCount ++;
		}
	} else if (ACT_Solenoid_Context.EyeMode == ACT_SOENOID_EYE_MODE_INACTIVE) {
		ACT_Solenoid_Context.SolenoidActiveStartTick = IO_SysTickTimer_GetTicks();
		IO_Pwm_OneShotTcc1(CFG_Config_GetUserSettingDwellTicks() * (1000000 / IO_SYSTICKTIMER_TICKS_PER_SEC));
		ACT_Solenoid_Context.ShotCount ++;	
	}
}


uint32_t ACT_Solenoid_GetSolenoidActiveStartTick(void) {
	return ACT_Solenoid_Context.SolenoidActiveStartTick;
}


uint32_t ACT_Solenoid_GetShotCount(void) {
	return ACT_Solenoid_Context.ShotCount;
}


void ACT_Solenoid_SetEyesActive(void) {
	ACT_Solenoid_Context.EyeMode = ACT_SOENOID_EYE_MODE_ACTIVE;
}


void ACT_Solenoid_SetEyesInActive(void) {
	ACT_Solenoid_Context.EyeMode = ACT_SOENOID_EYE_MODE_INACTIVE;
}

void ACT_Solenoid_SetShotCount(uint32_t ShotCount) {
	ACT_Solenoid_Context.ShotCount = ShotCount;
}