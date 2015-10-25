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


#ifndef ACT_SOLENOID_H_
#define ACT_SOLENOID_H_


#define ACT_SOENOID_EYE_MODE_ACTIVE   0
#define ACT_SOENOID_EYE_MODE_INACTIVE 1

void ACT_Solenoid_Initialize(void);

void ACT_Solenoid_Update(void);

void ACT_Solenoid_FireShot(void);

uint32_t ACT_Solenoid_GetSolenoidActiveStartTick(void);

uint32_t ACT_Solenoid_GetShotCount(void);

void ACT_Solenoid_SetEyesActive(void);

void ACT_Solenoid_SetEyesInActive(void);

void ACT_Solenoid_SetShotCount(uint32_t ShotCount);


#endif /* ACT_SOLENOID_H_ */