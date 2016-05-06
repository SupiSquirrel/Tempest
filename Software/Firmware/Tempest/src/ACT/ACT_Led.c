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


typedef struct {
   uint32_t NextUpdate;
   
   uint32_t ActStep;
   
   const ACT_Led_StepBrightness_Type* StepBrightnessR;
   const ACT_Led_StepBrightness_Type* StepBrightnessG;
   const ACT_Led_StepBrightness_Type* StepBrightnessB;
   const ACT_Led_StepBrightness_Type* StepBrightnessPwrLed;
   
   uint32_t TimeOffsets[4];
   
} ACT_Led_Context_Type;

static ACT_Led_Context_Type ACT_Led_Context;


const ACT_Led_StepBrightness_Type ACT_Led_Pattern_Off = {
   .StepBrightness = {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
};

const ACT_Led_StepBrightness_Type ACT_Led_Pattern_On = {
   .StepBrightness = {100u, 100u, 100u, 100u, 100u, 100u, 100u, 100u, 100u, 100u, 100u, 100u, 100u, 100u, 100u, 100u, 100u, 100u, 100u, 100u, 100u, 100u, 100u, 100u, 100u, 100u, 100u, 100u, 100u, 100u, 100u, 100u},
};

const ACT_Led_StepBrightness_Type ACT_Led_Pattern_Blink = {
   .StepBrightness = {100u, 100u, 100u, 100u, 0u, 0u, 0u, 0u, 100u, 100u, 100u, 100u, 0u, 0u, 0u, 0u, 100u, 100u, 100u, 100u, 0u, 0u, 0u, 0u, 100u, 100u, 100u, 100u, 0u, 0u, 0u, 0u},
};

const ACT_Led_StepBrightness_Type ACT_Led_Pattern_Flash = {
   .StepBrightness = {100u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
};

const ACT_Led_StepBrightness_Type ACT_Led_Pattern_FlashLong = {
	.StepBrightness = {100u, 100u, 100u, 100u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
};

const ACT_Led_StepBrightness_Type ACT_Led_Pattern_FastFlash = {
   .StepBrightness = {100u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 100u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 100u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 100u, 0u, 0u, 0u, 0u, 0u, 0u, 0u},
};



void ACT_Led_Initialize(void) {

   ACT_Led_Context.NextUpdate = 0;
   
   ACT_Led_Context.ActStep    = 0;
   
   ACT_Led_Context.StepBrightnessR      = &ACT_Led_Pattern_Off;
   ACT_Led_Context.StepBrightnessG      = &ACT_Led_Pattern_Off;
   ACT_Led_Context.StepBrightnessB      = &ACT_Led_Pattern_Off;
   ACT_Led_Context.StepBrightnessPwrLed = &ACT_Led_Pattern_Off;
   
   ACT_Led_Context.TimeOffsets[0] = 0;
   ACT_Led_Context.TimeOffsets[1] = 0;
   ACT_Led_Context.TimeOffsets[2] = 0;
   ACT_Led_Context.TimeOffsets[3] = 0;
}


void ACT_Led_Update(void) {

   // update led every 100 ms
   if (IO_SysTickTimer_GetTicks() >= ACT_Led_Context.NextUpdate) {
   
      // calculate next update point 
      ACT_Led_Context.NextUpdate = IO_SysTickTimer_GetTicks() + IO_SYSTICKTIMER_TICKS_PER_SEC / 50;
      
      // only 0% / 100% supported at the moment.
      // check if led shall be on or off for current step
      if ((*ACT_Led_Context.StepBrightnessR).StepBrightness[(ACT_Led_Context.ActStep + ACT_Led_Context.TimeOffsets[ACT_LED_RED]) % ACT_LED_NUM_OF_STEPS] != 0) {
         IO_Discretes_SetOutputActive(IO_DISCRETES_CHANNEL_LED_RED);
      } else {
         IO_Discretes_SetOutputInactive(IO_DISCRETES_CHANNEL_LED_RED);
      }
      
      if ((*ACT_Led_Context.StepBrightnessG).StepBrightness[(ACT_Led_Context.ActStep + ACT_Led_Context.TimeOffsets[ACT_LED_GREEN]) % ACT_LED_NUM_OF_STEPS] != 0) {
         IO_Discretes_SetOutputActive(IO_DISCRETES_CHANNEL_LED_GREEN);
      } else {
         IO_Discretes_SetOutputInactive(IO_DISCRETES_CHANNEL_LED_GREEN);
      }

      if ((*ACT_Led_Context.StepBrightnessB).StepBrightness[(ACT_Led_Context.ActStep + ACT_Led_Context.TimeOffsets[ACT_LED_BLUE]) % ACT_LED_NUM_OF_STEPS] != 0) {
         IO_Discretes_SetOutputActive(IO_DISCRETES_CHANNEL_LED_BLUE);
      } else {
         IO_Discretes_SetOutputInactive(IO_DISCRETES_CHANNEL_LED_BLUE);
      }
	  
      if ((*ACT_Led_Context.StepBrightnessPwrLed).StepBrightness[(ACT_Led_Context.ActStep + ACT_Led_Context.TimeOffsets[ACT_LED_BLUE]) % ACT_LED_NUM_OF_STEPS] != 0) {
	      IO_Discretes_SetOutputActive(IO_DISCRETES_CHANNEL_LED_POWER);
	      } else {
	      IO_Discretes_SetOutputInactive(IO_DISCRETES_CHANNEL_LED_POWER);
      }	     
      
      // step wrap-around
      ACT_Led_Context.ActStep++;
	  ACT_Led_Context.ActStep = ACT_Led_Context.ActStep % ACT_LED_NUM_OF_STEPS;
   }
   
}


void ACT_Led_SetStepBrightnessR(const ACT_Led_StepBrightness_Type* NewStepBrightnessR) {
   ACT_Led_Context.StepBrightnessR = NewStepBrightnessR;
}


void ACT_Led_SetStepBrightnessG(const ACT_Led_StepBrightness_Type* NewStepBrightnessG) {
   ACT_Led_Context.StepBrightnessG = NewStepBrightnessG;
}


void ACT_Led_SetStepBrightnessB(const ACT_Led_StepBrightness_Type* NewStepBrightnessB) {
   ACT_Led_Context.StepBrightnessB = NewStepBrightnessB;
}

void ACT_Led_SetStepBrightnessPwr(const ACT_Led_StepBrightness_Type* NewStepBrightnessPwrLed) {
	ACT_Led_Context.StepBrightnessPwrLed = NewStepBrightnessPwrLed;
}


void ACT_Led_SetTimeOffsets(uint32_t LedIndex, uint32_t Value) {
	
	if ((LedIndex < 4) && (Value < ACT_LED_NUM_OF_STEPS)) {
		ACT_Led_Context.TimeOffsets[LedIndex] = Value;
	}
}