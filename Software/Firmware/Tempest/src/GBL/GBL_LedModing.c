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
#include "IO_Discretes.h"
#include "IO_SysTickTimer.h"
#include "CFG_Config.h"
#include "ACT_Solenoid.h"
#include "TRG_Trigger.h"
#include "GBL_Defines.h"
#include "IO_SerialUart.h"
#include "FM_DetermineRof.h"
#include "ACT_Led.h"
#include "FM_FireMode.h"
#include "COM_CommandInterpreter.h"
#include "GBL_LedModing.h"
#include "COM_Ams001.h"
#include "ACT_Eye.h"


typedef struct {
	uint32_t State;

} GBL_LedModing_Context_Type;

static GBL_LedModing_Context_Type GBL_LedModing_Context;




void GBL_LedModing_Initialize(void) {
	GBL_LedModing_Context.State = 0;
}


void GBL_LedModing_Update(void) {
	if (COM_Ams001_GetIsConnected()) {
		ACT_Led_SetStepBrightnessB(&ACT_Led_Pattern_FlashLong);
	} else {
		ACT_Led_SetStepBrightnessB(&ACT_Led_Pattern_Off);
	}
	
	if (CFG_Config_GetUserSettingEyesOn() == ACT_SOENOID_EYE_MODE_INACTIVE) {
		ACT_Led_SetStepBrightnessR(&ACT_Led_Pattern_FlashLong);
		ACT_Led_SetTimeOffsets(ACT_LED_RED, ACT_LED_NUM_OF_STEPS / 2);
		ACT_Led_SetStepBrightnessG(&ACT_Led_Pattern_FlashLong);
		ACT_Led_SetTimeOffsets(ACT_LED_GREEN, ACT_LED_NUM_OF_STEPS / 2);
		
		ACT_Led_SetStepBrightnessPwr(&ACT_Led_Pattern_FlashLong);			
	} else {
		if (ACT_Eye_GetBallDetected()) {
			
			ACT_Led_SetStepBrightnessR(&ACT_Led_Pattern_FlashLong);
			ACT_Led_SetTimeOffsets(ACT_LED_RED, ACT_LED_NUM_OF_STEPS / 2);
			
			ACT_Led_SetStepBrightnessG(&ACT_Led_Pattern_Off);
			ACT_Led_SetTimeOffsets(ACT_LED_GREEN, ACT_LED_NUM_OF_STEPS / 2);
			
			ACT_Led_SetStepBrightnessPwr(&ACT_Led_Pattern_Flash);
						
		} else {
			
			ACT_Led_SetStepBrightnessR(&ACT_Led_Pattern_Off);
			ACT_Led_SetTimeOffsets(ACT_LED_RED, ACT_LED_NUM_OF_STEPS / 2);
			
			ACT_Led_SetStepBrightnessG(&ACT_Led_Pattern_FlashLong);
			ACT_Led_SetTimeOffsets(ACT_LED_GREEN, ACT_LED_NUM_OF_STEPS / 2);
			
			ACT_Led_SetStepBrightnessPwr(&ACT_Led_Pattern_FastFlash);			
		}
	}
}