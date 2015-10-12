/*
 * CFG_Config.c
 *
 * Created: 15.04.2015 21:22:53
 *  Author: Squirrel
 */ 

#include <asf.h>
#include "TRG_Trigger.h"
#include "IO_Discretes.h"
#include "IO_SysTickTimer.h"

typedef struct {
	// maximum rof with eyes on [Hz]
	float UserSettingMaximumRofEyesOn;
	
	// maximum rof with eyes off [Hz]
	float UserSettingMaximumRofEyesOff;
	
	// ramp kick in [Hz]
	float UserSettingRampKickinRof;
	
	// dwell [ms]
	float UserSettingDwell;
	
	// ramp restart time [s]
	float UserSettingRampRestartTime;
	
	// debounce setting [ ]
	uint32_t UserSettingDebounce;
	
	// sleep [min]
	uint32_t UserSettingSleep;
	
	// firing mode
	uint32_t UserSettingFiringMode;
	
} CFG_Config_Context_Type;

static CFG_Config_Context_Type CFG_Config_Context;


void CFG_Config_Initialize(void) {
	CFG_Config_Context.UserSettingMaximumRofEyesOn  = 15.0;
	CFG_Config_Context.UserSettingMaximumRofEyesOff =  7.5;
	CFG_Config_Context.UserSettingRampKickinRof     =  5.0;
	CFG_Config_Context.UserSettingDwell             = 12.0;
	CFG_Config_Context.UserSettingRampRestartTime   =  0.0;
	CFG_Config_Context.UserSettingDebounce          =   10u;
	CFG_Config_Context.UserSettingSleep             =   10u;
	CFG_Config_Context.UserSettingFiringMode        =    0;
}


uint32_t CFG_Config_GetUserSettingMaximumRofEyesOnTicks() {
	return (uint32_t)( ((float)IO_SYSTICKTIMER_TICK_FREQUENCY) / CFG_Config_Context.UserSettingMaximumRofEyesOn);
}

uint32_t CFG_Config_GetUserSettingMaximumRofEyesOffTicks() {
	return (uint32_t)( ((float)IO_SYSTICKTIMER_TICK_FREQUENCY) / CFG_Config_Context.UserSettingMaximumRofEyesOff);
}

uint32_t CFG_Config_GetUserSettingRampKickinRofTicks() {
	return (uint32_t)( ((float)IO_SYSTICKTIMER_TICK_FREQUENCY) / CFG_Config_Context.UserSettingRampKickinRof);
}

uint32_t CFG_Config_GetUserSettingDwellTicks() {
	return (uint32_t)( CFG_Config_Context.UserSettingDwell / (1000.0 / ((float)IO_SYSTICKTIMER_TICK_FREQUENCY)));
}