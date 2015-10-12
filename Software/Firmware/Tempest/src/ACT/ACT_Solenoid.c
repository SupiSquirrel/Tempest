/*
 * ACT_Solenoid.c
 *
 * Created: 15.04.2015 22:03:26
 *  Author: Squirrel
 */ 


#include <asf.h>
#include "TRG_Trigger.h"
#include "IO_Discretes.h"
#include "IO_SysTickTimer.h"
#include "ACT_Solenoid.h"
#include "CFG_Config.h"

typedef struct {
	uint32_t SolenoidActiveStartTick;
	bool SolenoidActive;
} ACT_Solenoid_Context_Type;

static ACT_Solenoid_Context_Type ACT_Solenoid_Context;


void ACT_Solenoid_Initialize(void) {
	IO_Discretes_SetOutputInactive(IO_DISCRETES_CHANNEL_SOLENOID);
	ACT_Solenoid_Context.SolenoidActive          = false;
	ACT_Solenoid_Context.SolenoidActiveStartTick = 0;
}



void ACT_Solenoid_Update(void) {
	if (ACT_Solenoid_Context.SolenoidActive) {
		if (IO_SysTickTimer_GetTicks() > ACT_Solenoid_Context.SolenoidActiveStartTick + CFG_Config_GetUserSettingDwellTicks()) {
			ACT_Solenoid_Context.SolenoidActive = false;
			IO_Discretes_SetOutputInactive(IO_DISCRETES_CHANNEL_SOLENOID);
		}
	}
}


void ACT_Solenoid_FireShot(void) {
	if (!ACT_Solenoid_Context.SolenoidActive) {
		ACT_Solenoid_Context.SolenoidActive          = true;
		ACT_Solenoid_Context.SolenoidActiveStartTick = IO_SysTickTimer_GetTicks();
		
		// instant activation - not synced to tick edge
		// this leads to a jitter of 1/IO_SYSTICKTIMER_TICK_FREQUENCY [s]
		IO_Discretes_SetOutputActive(IO_DISCRETES_CHANNEL_SOLENOID);
	}
}