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


#ifndef CFG_CONFIG_H_
#define CFG_CONFIG_H_


// maps to 'a'-'i'
#define CFG_CONFIG_NUM_USER_MODIFIABLE_PARAMETER               9

#define CFG_CONFIG_USER_MODIFIABLE_PARAMETER_MAX_ROF_EYES_ON   0
#define CFG_CONFIG_USER_MODIFIABLE_PARAMETER_MAX_ROF_EYES_OFF  1
#define CFG_CONFIG_USER_MODIFIABLE_PARAMETER_RAMP_KICKIN_ROF   2
#define CFG_CONFIG_USER_MODIFIABLE_PARAMETER_RAMP_KICKIN_PULLS 3
#define CFG_CONFIG_USER_MODIFIABLE_PARAMETER_DWELL             4
#define CFG_CONFIG_USER_MODIFIABLE_PARAMETER_DEBOUNCE          5
#define CFG_CONFIG_USER_MODIFIABLE_PARAMETER_SLEEP             6
#define CFG_CONFIG_USER_MODIFIABLE_PARAMETER_FIRING_MODE       7
#define CFG_CONFIG_USER_MODIFIABLE_PARAMETER_EYES_ON           8


void CFG_Config_Initialize(void);

uint32_t CFG_Config_GetParameterDefaultsLimitsMax(uint32_t Index);
uint32_t CFG_Config_GetParameterDefaultsLimitsMin(uint32_t Index);

uint32_t CFG_Config_GetUserParameter(uint32_t Index);
uint32_t CFG_Config_SetUserParameter(uint32_t Index, uint32_t Value);

uint32_t CFG_Config_GetUserSettingMaximumRofEyesOnTicks(void);
uint32_t CFG_Config_GetUserSettingMaximumRofEyesOffTicks(void);
uint32_t CFG_Config_GetUserSettingRampKickinRofTicks(void);
uint32_t CFG_Config_GetUserSettingDwellTicks(void);

uint32_t CFG_Config_GetUserSettingFiringMode(void);
uint32_t CFG_Config_GetUserSettingRampKickinPulls(void);



#endif /* CFG_CONFIG_H_ */