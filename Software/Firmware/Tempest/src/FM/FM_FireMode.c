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
#include "FM_FireMode.h"

#define RAMP_STATE_RAMP_ACTIVE                 1
#define RAMP_STATE_RAMP_WAIT_FOR_REQUIRED_ROF  2


typedef struct {

   uint32_t UserMode;
   
   uint32_t PrevTriggerPulledTimestampTicks;
   
   uint32_t RampState;
   uint32_t RampTriggerPullsAtRequiredRof;
   uint32_t RampStartedTimestampTicks;

} FM_FireMode_Context_Type;

static FM_FireMode_Context_Type FM_FireMode_Context;



void FM_FireMode_Initialize(void) {
	
   FM_FireMode_Context.UserMode                        = CFG_Config_GetUserSettingFiringMode();
   
   FM_FireMode_Context.RampState                       = RAMP_STATE_RAMP_WAIT_FOR_REQUIRED_ROF;
   FM_FireMode_Context.RampTriggerPullsAtRequiredRof   = 0;
   FM_FireMode_Context.PrevTriggerPulledTimestampTicks = 0;
}

static void FM_FireMode_HandleSemi(void) {

   if (TRG_Trigger_GetEdge() != TRG_TRIGGER_EDGE_NONE) {
      if (TRG_Trigger_GetEdge() == TRG_TRIGGER_EDGE_PULL) {
         ACT_Solenoid_FireShot();
      } else {
         // trigger released
         // TriggerPulledTicks = TRG_Trigger_GetReleaseTimestampTicks() - TRG_Trigger_GetPullTimestampTicks();
      }
   }
}

static void FM_FireMode_HandleRamp(void) {

   uint32_t ActTriggerPulledTimestampTicks;
   
   ActTriggerPulledTimestampTicks = TRG_Trigger_GetPullTimestampTicks();

   if (TRG_Trigger_GetEdge() == TRG_TRIGGER_EDGE_PULL) {
   
      // time since the last pull within kick-in rof limit?
      if ((ActTriggerPulledTimestampTicks - FM_FireMode_Context.PrevTriggerPulledTimestampTicks) <= CFG_Config_GetUserSettingRampKickinRofTicks()) {
         FM_FireMode_Context.RampTriggerPullsAtRequiredRof ++;
      } else {
         FM_FireMode_Context.RampTriggerPullsAtRequiredRof = 0;
      }
      
      FM_FireMode_Context.PrevTriggerPulledTimestampTicks = ActTriggerPulledTimestampTicks;
      
   }
   
   if (IO_SysTickTimer_GetTicks() > ActTriggerPulledTimestampTicks + CFG_Config_GetUserSettingRampKickinRofTicks()) {
      FM_FireMode_Context.RampTriggerPullsAtRequiredRof = 0;
   }
   
   
   switch (FM_FireMode_Context.RampState) {
   
      case RAMP_STATE_RAMP_WAIT_FOR_REQUIRED_ROF:
      
               if (TRG_Trigger_GetEdge() == TRG_TRIGGER_EDGE_PULL) {
                  ACT_Solenoid_FireShot();
               }
               
               if (FM_FireMode_Context.RampTriggerPullsAtRequiredRof >= CFG_Config_GetUserSettingRampKickinPulls()) {
                  FM_FireMode_Context.RampState = RAMP_STATE_RAMP_ACTIVE;
               }
               
               break;
               
      case RAMP_STATE_RAMP_ACTIVE:
      
               if (IO_SysTickTimer_GetTicks() >= (ACT_Solenoid_GetSolenoidActiveStartTick() + CFG_Config_GetUserSettingMaximumRofEyesOnTicks())) {
                  ACT_Solenoid_FireShot();
               }
               
               if (FM_FireMode_Context.RampTriggerPullsAtRequiredRof == 0) {
                  FM_FireMode_Context.RampState = RAMP_STATE_RAMP_WAIT_FOR_REQUIRED_ROF;
               }
               break;
   }
}

/*
static void FM_FireMode_HandleAuto(void) {
}*/

float FM_FireMode_GetSolenoidRof(void) {
	if (FM_FireMode_Context.RampState == RAMP_STATE_RAMP_ACTIVE) {
		return ( (float)IO_SYSTICKTIMER_TICKS_PER_SEC / (CFG_Config_GetUserSettingMaximumRofEyesOnTicks()) );
	} else {
		return FM_DetermineRof_GetRof();
	}
}

void FM_FireMode_Update(void) {
	
   // test
   FM_FireMode_Context.UserMode                        = CFG_Config_GetUserSettingFiringMode();

   switch (FM_FireMode_Context.UserMode) {
      case FM_FIREMODE_MODE_SEMI:
                  FM_FireMode_HandleSemi();
                  break;
      case FM_FIREMODE_MODE_RAMP:
                  FM_FireMode_HandleRamp();
                  break;                  
      case FM_FIREMODE_MODE_AUTO:
				  // auto removed
				  // is now burst
                  // FM_FireMode_HandleAuto();
				  
				  FM_FireMode_HandleSemi();
                  break;
      default:
                  break;
   }
}