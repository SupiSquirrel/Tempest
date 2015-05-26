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


#ifndef FM_FIREMODE_H_
#define FM_FIREMODE_H_


#define FM_FIREMODE_MODE_SEMI          1
#define FM_FIREMODE_MODE_RAMP          2
#define FM_FIREMODE_MODE_AUTO          3


void FM_FireMode_Initialize(void);

void FM_FireMode_Update(void);

float FM_FireMode_GetSolenoidRof(void);


#endif /* FM_FIREMODE_H_ */