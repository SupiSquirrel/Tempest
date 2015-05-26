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


#ifndef ACT_BATTERYMONITOR_H_
#define ACT_BATTERYMONITOR_H_


void ACT_BatteryMonitor_Initialize(void);

void ACT_BatteryMonitor_Update(void);

float ACT_BatteryMonitor_GetBatteryVoltage(void);

bool ACT_BatteryMonitor_GetButtonPushed(void);

bool ACT_BatteryMonitor_GetButtonPushedLongerThan(uint32_t ticks);

void ACT_BatteryMonitor_ResetButtonPushedTimer(void);


#endif /* ACT_BATTERYMONITOR_H_ */