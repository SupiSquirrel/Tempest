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


#ifndef IO_DISCRETES_H_
#define IO_DISCRETES_H_

#define IO_DISCRETES_CHANNEL_TRIGGER             1
#define IO_DISCRETES_CHANNEL_AMS_STATUSLED       2
#define IO_DISCRETES_CHANNEL_TOURNAMENT_SWITCH   3
#define IO_DISCRETES_CHANNEL_MAIN_SWITCH         4
#define IO_DISCRETES_CHANNEL_OBS_RX              5
#define IO_DISCRETES_CHANNEL_UP_SWITCH           6

#define IO_DISCRETES_CHANNEL_LED_RED             1
#define IO_DISCRETES_CHANNEL_LED_GREEN	         2
#define IO_DISCRETES_CHANNEL_LED_BLUE	         3
#define IO_DISCRETES_CHANNEL_AMS_RESET	         4
#define IO_DISCRETES_CHANNEL_AMS_MODESEL_STREAM	 5
#define IO_DISCRETES_CHANNEL_SOLENOID            6
#define IO_DISCRETES_CHANNEL_OBS_TX              7
#define IO_DISCRETES_CHANNEL_POWER_ENABLE        8
#define IO_DISCRETES_CHANNEL_LED_POWER	         9


void IO_Discretes_Initialize(void);

bool IO_Discretes_GetInputIsSet(uint32_t channel);

void IO_Discretes_SetOutputActive(uint32_t channel);
void IO_Discretes_SetOutputInactive(uint32_t channel);

#endif /* IO_DISCRETES_H_ */