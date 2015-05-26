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
#include "CFG_Config.h"
#include "ACT_Solenoid.h"
#include "TRG_Trigger.h"
#include "GBL_Defines.h"
#include "IO_SerialUart.h"
#include "FM_DetermineRof.h"
#include "ACT_Led.h"
#include "FM_FireMode.h"
#include "COM_CommandInterpreter.h"
#include "GBL_LedModing.h"
#include "COM_Ams001.h"


typedef struct {
	uint32_t State;
	bool IsConnected;

} COM_Ams001_Context_Type;

static COM_Ams001_Context_Type COM_Ams001_Context;



void COM_Ams001_Initialize(void) {
		
		IO_Discretes_SetOutputActive(IO_DISCRETES_CHANNEL_AMS_RESET);
		IO_SysTickTimer_WaitMs(100);
		
		IO_Discretes_SetOutputInactive(IO_DISCRETES_CHANNEL_AMS_RESET);
		IO_SysTickTimer_WaitMs(500);
		
		// 	uint8_t AmsCommandStrGetAl[] = "get al\r\n";
		// 	IO_SerialUart_Write_Blocking(AmsCommandStrGetAl, sizeof(AmsCommandStrGetAl));

		IO_Discretes_SetOutputActive(IO_DISCRETES_CHANNEL_AMS_MODESEL_STREAM);
}


void COM_Ams001_Update(void) {
	COM_Ams001_Context.IsConnected = IO_Discretes_GetInputIsSet(IO_DISCRETES_CHANNEL_AMS_STATUSLED);
}


bool COM_Ams001_GetIsConnected(void) {
	return COM_Ams001_Context.IsConnected;
}