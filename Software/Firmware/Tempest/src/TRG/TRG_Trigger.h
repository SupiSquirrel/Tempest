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


#ifndef TRG_TRIGGER_H_
#define TRG_TRIGGER_H_


#define TRG_TRIGGER_PULLED 1
#define TRG_TRIGGER_RELEASED 2

#define TRG_TRIGGER_DEBOUNCE_TICKS (IO_SYSTICKTIMER_TICK_FREQUENCY/1000 * 10) // 10ms

#define TRG_TRIGGER_EDGE_NONE 0
#define TRG_TRIGGER_EDGE_PULL 1
#define TRG_TRIGGER_EDGE_RELEASE 2


void TRG_Trigger_Initialize(void);
void TRG_Trigger_Update(void);

void TRG_Trigger_ClearEdge(void);
uint32_t TRG_Trigger_GetEdge(void);
uint32_t TRG_Trigger_GetReleaseTimestampTicks(void);
uint32_t TRG_Trigger_GetPullTimestampTicks(void);
uint32_t TRG_Trigger_GetState(void);


#endif /* TRG_TRIGGER_H_ */