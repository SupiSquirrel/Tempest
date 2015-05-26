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
#include "IO_Analogs.h"
#include "ACT_BatteryMonitor.h"


#define ACT_BATTERYMONITOR_ADC_TO_VOLTAGE_FACTOR 0.009221311
#define ACT_BATTERYMONITOR_VOLTAGE_TO_ADC_FACTOR 108.0
#define ACT_BATTERYMONITOR_VOLTAGE_CONSTANT_ADD  0.213


typedef struct {
	float BatteryVoltage;
	uint32_t PushedCounter;
	uint32_t PushedStartTick;
} ACT_BatteryMonitor_Context_Type;

static ACT_BatteryMonitor_Context_Type ACT_BatteryMonitor_Context;



void ACT_BatteryMonitor_Initialize(void) {
	ACT_BatteryMonitor_Context.BatteryVoltage  = 0.0;
	ACT_BatteryMonitor_Context.PushedCounter   = 0u;
	ACT_BatteryMonitor_Context.PushedStartTick = 0u;
}


void ACT_BatteryMonitor_Update(void) {
	
	uint16_t AdcValue = IO_Analogs_GetChannel(IO_ANALOGS_CHANNEL_PUSHBUTTON_VCC9);
	
	// check if button is/was pressed (if Vcc > ~5V)
	if (AdcValue > 512) {
		ACT_BatteryMonitor_Context.BatteryVoltage = (float)AdcValue / ACT_BATTERYMONITOR_VOLTAGE_TO_ADC_FACTOR + ACT_BATTERYMONITOR_VOLTAGE_CONSTANT_ADD;
		ACT_BatteryMonitor_Context.PushedCounter++;		
	} else {
		ACT_BatteryMonitor_Context.PushedCounter   = 0u;
		ACT_BatteryMonitor_Context.PushedStartTick = IO_SysTickTimer_GetTicks();
	}
}


float ACT_BatteryMonitor_GetBatteryVoltage(void) {
	return ACT_BatteryMonitor_Context.BatteryVoltage;
}


bool ACT_BatteryMonitor_GetButtonPushed(void) {
	return ACT_BatteryMonitor_Context.PushedCounter > 0;
}


bool ACT_BatteryMonitor_GetButtonPushedLongerThan(uint32_t ticks) {
	return (IO_SysTickTimer_GetTicks() > ACT_BatteryMonitor_Context.PushedStartTick + ticks);
}


void ACT_BatteryMonitor_ResetButtonPushedTimer(void) {
	ACT_BatteryMonitor_Context.PushedStartTick = IO_SysTickTimer_GetTicks();
}