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

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */


#include <asf.h>
#include "IO_Discretes.h"
#include "IO_SysTickTimer.h"
#include "IO_ExtInt.h"
#include "IO_Pwm.h"
#include "IO_Analogs.h"
#include "IO_SerialUart.h"
#include "CFG_Config.h"
#include "TRG_Trigger.h"
#include "FM_DetermineRof.h"
#include "ACT_Led.h"
#include "ACT_Eye.h"
#include "ACT_Solenoid.h"
#include "ACT_BatteryMonitor.h"
#include "FM_FireMode.h"
#include "COM_CommandInterpreter.h"
#include "COM_Ams001.h"
#include "GBL_Defines.h"
#include "GBL_LedModing.h"
#include "IO_Eeprom.h"


int main(void)
{

	bool WaitForPowerOn;
	
	uint32_t UpSwitchPushedCounter    = 0;
	uint32_t UpSwitchPushedStartTick  = 0;
	
	system_init();
	
	IO_Discretes_Initialize();
	IO_SysTickTimer_Initialize();
	IO_SerialUart_Initialize();
	IO_ExtInt_Initialize();
	IO_Pwm_Initialize();
	IO_Analogs_Initialize();
	IO_Eeprom_Initialize();
	
	
	CFG_Config_Initialize();
	
	ACT_Solenoid_Initialize();
	
	ACT_BatteryMonitor_Initialize();

	TRG_Trigger_Initialize();
	
	ACT_Eye_Initialize();
	
	FM_FireMode_Initialize();

	ACT_Led_Initialize();
	
	COM_CommandInterpreter_Initialize();
	
	GBL_LedModing_Initialize();
	
	COM_Ams001_Initialize();
	


	WaitForPowerOn = true;
	
	do {
		IO_Analogs_Update();
		ACT_Led_Update();
		ACT_BatteryMonitor_Update();
		
		if (ACT_BatteryMonitor_GetButtonPushed()) {
			ACT_Led_SetStepBrightnessR(&ACT_Led_Pattern_FastFlash);
			ACT_Led_SetStepBrightnessG(&ACT_Led_Pattern_FastFlash);
		}
		
		if (ACT_BatteryMonitor_GetButtonPushedLongerThan(IO_SYSTICKTIMER_TICKS_PER_SEC / 2)) {
			WaitForPowerOn = false;
		}
		
	} while (WaitForPowerOn);
	
	// enable dcdc power regulator
	IO_Discretes_SetOutputActive(IO_DISCRETES_CHANNEL_POWER_ENABLE);
	
	ACT_BatteryMonitor_ResetButtonPushedTimer();
	
	
	uint32_t nextSecTicks = IO_SysTickTimer_GetTicks() + IO_SYSTICKTIMER_TICKS_PER_SEC;

	while (true) {
		// debug: measure runtime of main loop
		port_pin_set_output_level(PIN_PA18, true);
		
		IO_Analogs_Update();
		
		ACT_BatteryMonitor_Update();
		
		ACT_Eye_Update();
		
		TRG_Trigger_Update();
		
		ACT_Solenoid_Update();
		
		FM_FireMode_Update();
		
		ACT_Led_Update();
		
		FM_DetermineRof_Update();
		
		COM_CommandInterpreter_Update();
		
		GBL_LedModing_Update();
		
		COM_Ams001_Update();
		
		IO_SerialUart_Update(); 
		
		// debug printout
		if ((IO_SysTickTimer_GetTicks() >= nextSecTicks) && (false)) {
			nextSecTicks = IO_SysTickTimer_GetTicks() + IO_SYSTICKTIMER_TICKS_PER_SEC / 10;
			// printf("");
		}
		
		if (IO_Discretes_GetInputIsSet(IO_DISCRETES_CHANNEL_UP_SWITCH)) {
			UpSwitchPushedCounter ++;
		} else {
			UpSwitchPushedCounter   = 0;
			UpSwitchPushedStartTick = IO_SysTickTimer_GetTicks();
		}
		
		if (IO_SysTickTimer_GetTicks() > (UpSwitchPushedStartTick + IO_SYSTICKTIMER_TICKS_PER_SEC * 1)) {
			// toggle eyes
			if (CFG_Config_GetUserSettingEyesOn() == ACT_SOENOID_EYE_MODE_ACTIVE) {
				CFG_Config_SetUserParameter(CFG_CONFIG_USER_MODIFIABLE_PARAMETER_EYES_ON, ACT_SOENOID_EYE_MODE_INACTIVE);
			} else {
				CFG_Config_SetUserParameter(CFG_CONFIG_USER_MODIFIABLE_PARAMETER_EYES_ON, ACT_SOENOID_EYE_MODE_ACTIVE);
			}
			
			UpSwitchPushedStartTick = IO_SysTickTimer_GetTicks();
		}
		
		// if button is pressed > 5s, power down system.
		if (ACT_BatteryMonitor_GetButtonPushedLongerThan(IO_SYSTICKTIMER_TICKS_PER_SEC * 3)) {
			
			CFG_Config_Save();
			
			ACT_Led_SetStepBrightnessB(&ACT_Led_Pattern_Off);
			ACT_Led_SetStepBrightnessR(&ACT_Led_Pattern_FastFlash);
			ACT_Led_SetStepBrightnessG(&ACT_Led_Pattern_Off);
			
			IO_Discretes_SetOutputInactive(IO_DISCRETES_CHANNEL_POWER_ENABLE);
			
			do {
			} while (true);
		}
		
		// debug: measure runtime of main loop
		port_pin_set_output_level(PIN_PA18, false);
	}
}
