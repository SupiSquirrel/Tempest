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
#include "CFG_Config.h"
#include "FM_FireMode.h"
#include "ACT_Solenoid.h"
#include "IO_Eeprom.h"

#include <string.h>


typedef struct {
	uint32_t Min;
	uint32_t Max;
	uint32_t Default;
} CFG_Config_ParameterConsts_Type;


static const CFG_Config_ParameterConsts_Type CFG_Config_ParameterDefaultsLimits[CFG_CONFIG_NUM_USER_MODIFIABLE_PARAMETER] = {
		{ // CFG_CONFIG_USER_MODIFIABLE_PARAMETER_MAX_ROF_EYES_ON
			.Max     = (uint32_t)((float)IO_SYSTICKTIMER_TICKS_PER_SEC / (float)1.0),
			.Min     = (uint32_t)((float)IO_SYSTICKTIMER_TICKS_PER_SEC / (float)20.0),
			.Default = (uint32_t)((float)IO_SYSTICKTIMER_TICKS_PER_SEC / (float)12.0),
		},
		{// CFG_CONFIG_USER_MODIFIABLE_PARAMETER_MAX_ROF_EYES_OFF
			.Max     = (uint32_t)((float)IO_SYSTICKTIMER_TICKS_PER_SEC / (float)1.0),
			.Min     = (uint32_t)((float)IO_SYSTICKTIMER_TICKS_PER_SEC / (float)20.0),
			.Default = (uint32_t)((float)IO_SYSTICKTIMER_TICKS_PER_SEC / (float)7.5),
		},
		{ // CFG_CONFIG_USER_MODIFIABLE_PARAMETER_RAMP_KICKIN_ROF
			.Max     = (uint32_t)((float)IO_SYSTICKTIMER_TICKS_PER_SEC / (float)1.0),
			.Min     = (uint32_t)((float)IO_SYSTICKTIMER_TICKS_PER_SEC / (float)20.0),
			.Default = (uint32_t)((float)IO_SYSTICKTIMER_TICKS_PER_SEC / (float)5.0),
		},
		{ // CFG_CONFIG_USER_MODIFIABLE_PARAMETER_RAMP_KICKIN_PULLS
			.Min     = 2u,
			.Max     = 5u,
			.Default = 3u,
		},
		{ // CFG_CONFIG_USER_MODIFIABLE_PARAMETER_DWELL
			.Min     = (uint32_t)((float)IO_SYSTICKTIMER_TICKS_PER_SEC * (float)( 5.0 / 1000.0)),
			.Max     = (uint32_t)((float)IO_SYSTICKTIMER_TICKS_PER_SEC * (float)(25.0 / 1000.0)),
			.Default = (uint32_t)((float)IO_SYSTICKTIMER_TICKS_PER_SEC * (float)(10.0 / 1000.0)),	
		},
		{ // CFG_CONFIG_USER_MODIFIABLE_PARAMETER_DEBOUNCE
			.Min     =  5u,
			.Max     = 20u,
			.Default = 10u,
		},
		{ // CFG_CONFIG_USER_MODIFIABLE_PARAMETER_SLEEP
			.Min     =  5u,
			.Max     = 30u,
			.Default = 10u,
		},
		{ // CFG_CONFIG_USER_MODIFIABLE_PARAMETER_FIRING_MODE
			.Min     = FM_FIREMODE_MODE_SEMI,
			.Max     = FM_FIREMODE_MODE_AUTO,
			.Default = FM_FIREMODE_MODE_RAMP,
		},
		{ // CFG_CONFIG_USER_MODIFIABLE_PARAMETER_EYES_ON
			.Min     = ACT_SOENOID_EYE_MODE_INACTIVE,
			.Max     = ACT_SOENOID_EYE_MODE_ACTIVE,
			.Default = ACT_SOENOID_EYE_MODE_ACTIVE,	
		}
};


typedef struct {

	uint32_t UserParameter[CFG_CONFIG_NUM_USER_MODIFIABLE_PARAMETER];
	uint32_t ShotCount;
	
} CFG_Config_Context_Type;

static CFG_Config_Context_Type CFG_Config_Context;


typedef struct {
	uint32_t UserParameter[CFG_CONFIG_NUM_USER_MODIFIABLE_PARAMETER];
	uint32_t ShotCount;
} CFG_Config_EepromInfo_Type;


void CFG_Config_Initialize(void) {
	uint32_t Counter;
	
	uint8_t PageData[EEPROM_PAGE_SIZE];
	CFG_Config_EepromInfo_Type EepromInfo;
	
	IO_Eeprom_ReadPage(0, PageData);		memcpy(&EepromInfo, &PageData, sizeof(CFG_Config_EepromInfo_Type));
	if (EepromInfo.ShotCount == 0xffffffff) {
		CFG_Config_Context.ShotCount = 0;
	} else {
		CFG_Config_Context.ShotCount = EepromInfo.ShotCount;
	}
	
	for (Counter = 0; Counter < CFG_CONFIG_NUM_USER_MODIFIABLE_PARAMETER; Counter++) {
		//CFG_Config_Context.UserParameter[Counter] = CFG_Config_ParameterDefaultsLimits[Counter].Default;
		if (!CFG_Config_SetUserParameter(Counter, EepromInfo.UserParameter[Counter])) {
			CFG_Config_Context.UserParameter[Counter] = CFG_Config_ParameterDefaultsLimits[Counter].Default;
		}
	}			
}


void CFG_Config_Save(void) {
	uint8_t PageData[EEPROM_PAGE_SIZE];
	CFG_Config_EepromInfo_Type EepromInfo;
	
	EepromInfo.ShotCount = ACT_Solenoid_GetShotCount();
	memcpy(&EepromInfo.UserParameter, &CFG_Config_Context.UserParameter, sizeof(CFG_Config_Context.UserParameter));
	
	memcpy(&PageData, &EepromInfo, sizeof(CFG_Config_EepromInfo_Type));
	IO_Eeprom_WritePage(0, PageData);
	IO_Eeprom_Commit();
}


uint32_t CFG_Config_GetShotCount(void) {
	return CFG_Config_Context.ShotCount;
}


void CFG_Config_LoadDefaults(void) {
	uint32_t Counter;
	
	for (Counter = 0; Counter < CFG_CONFIG_NUM_USER_MODIFIABLE_PARAMETER; Counter++) {
		CFG_Config_Context.UserParameter[Counter] = CFG_Config_ParameterDefaultsLimits[Counter].Default;
	}
	
}


uint32_t CFG_Config_GetUserParameter(uint32_t Index) {
	return CFG_Config_Context.UserParameter[Index];
}


uint32_t CFG_Config_GetParameterDefaultsLimitsMax(uint32_t Index) {
	return CFG_Config_ParameterDefaultsLimits[Index].Max;
}


uint32_t CFG_Config_GetParameterDefaultsLimitsMin(uint32_t Index) {
	return CFG_Config_ParameterDefaultsLimits[Index].Min;
}


uint32_t CFG_Config_SetUserParameter(uint32_t Index, uint32_t Value) {
	
	uint32_t Result;
	
	Result = 0;
	
	if (Index < CFG_CONFIG_NUM_USER_MODIFIABLE_PARAMETER) {
		if ((Value >=  CFG_Config_ParameterDefaultsLimits[Index].Min) && (Value <=  CFG_Config_ParameterDefaultsLimits[Index].Max)) {
			CFG_Config_Context.UserParameter[Index] = Value;
			Result = 1;
		}
	}
	return Result;	
}



uint32_t CFG_Config_GetUserSettingMaximumRofEyesOnTicks(void) {
	return CFG_Config_Context.UserParameter[CFG_CONFIG_USER_MODIFIABLE_PARAMETER_MAX_ROF_EYES_ON];
}


uint32_t CFG_Config_GetUserSettingMaximumRofEyesOffTicks(void) {
	return CFG_Config_Context.UserParameter[CFG_CONFIG_USER_MODIFIABLE_PARAMETER_MAX_ROF_EYES_OFF];
}


uint32_t CFG_Config_GetUserSettingRampKickinRofTicks(void) {
	return CFG_Config_Context.UserParameter[CFG_CONFIG_USER_MODIFIABLE_PARAMETER_RAMP_KICKIN_ROF];
}


uint32_t CFG_Config_GetUserSettingDwellTicks(void) {
	return CFG_Config_Context.UserParameter[CFG_CONFIG_USER_MODIFIABLE_PARAMETER_DWELL];
}


uint32_t CFG_Config_GetUserSettingFiringMode(void) {
   return CFG_Config_Context.UserParameter[CFG_CONFIG_USER_MODIFIABLE_PARAMETER_FIRING_MODE];
}


uint32_t CFG_Config_GetUserSettingRampKickinPulls(void) {
   return CFG_Config_Context.UserParameter[CFG_CONFIG_USER_MODIFIABLE_PARAMETER_RAMP_KICKIN_PULLS];
}

uint32_t CFG_Config_GetUserSettingEyesOn(void) {
	return CFG_Config_Context.UserParameter[CFG_CONFIG_USER_MODIFIABLE_PARAMETER_EYES_ON];
}

