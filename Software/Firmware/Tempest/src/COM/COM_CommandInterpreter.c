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
#include <string.h>
#include "IO_SysTickTimer.h"
#include "IO_SerialUart.h"
#include "COM_CommandInterpreter.h"
#include "CFG_Config.h"
#include "FM_DetermineRof.h"
#include "TRG_Trigger.h"
#include "COM_Ams001.h"
#include "FM_FireMode.h"
#include "ACT_Solenoid.h"
#include "ACT_Eye.h"
#include "ACT_BatteryMonitor.h"


#define COM_COMMANDINTERPRETER_BUFFER_SIZE 16

#define COM_COMMANDINTERPRETER_STATE_WAIT_FOR_START   1
#define COM_COMMANDINTERPRETER_STATE_WAIT_FOR_END     2
#define COM_COMMANDINTERPRETER_STATE_INIT             3

#define COM_COMMANDINTERPRETER_CMD_CHAR_START '+'
#define COM_COMMANDINTERPRETER_CMD_CHAR_END   '#'

#define COM_COMMANDINTERPRETER_CMD_CHAR_GET   'g'
#define COM_COMMANDINTERPRETER_CMD_CHAR_SET   's'

#define COM_COMMANDINTERPRETER_NUM_RUNTIME_PARAMETER 1

#define COM_COMMANDINTERPRETER_RUNTIME_PARAMETER_ROF          0
#define COM_COMMANDINTERPRETER_RUNTIME_PARAMETER_ROF_SOLENOID 1
#define COM_COMMANDINTERPRETER_RUNTIME_PARAMETER_MAX_ROF      2
#define COM_COMMANDINTERPRETER_RUNTIME_PARAMETER_VOLTAGE      3

#undef COM_COMMANDINTERPRETER_UPDATE_DEBUG
//#define COM_COMMANDINTERPRETER_UPDATE_DEBUG


typedef struct {
	uint32_t State;
	char Buffer[COM_COMMANDINTERPRETER_BUFFER_SIZE + 1];
	uint32_t WritePointer;
	
	uint32_t NextStatusOneUpdateTicks;
	uint32_t NextStatusTwoUpdateTicks;
} COM_CommandInterpreter_Context_Type;

static COM_CommandInterpreter_Context_Type COM_CommandInterpreter_Context;


static void COM_CommandInterpreter_ProcessCommand(void);
static void COM_CommandInterpreter_ProcessCommandGet(void);
static void COM_CommandInterpreter_SendGetResponse(char ParameterId, uint32_t Value);
static void COM_CommandInterpreter_SendStatusOneResponse(void);
static void COM_CommandInterpreter_SendStatusTwoResponse(void);



void COM_CommandInterpreter_Initialize(void) {
	COM_CommandInterpreter_Context.State        = COM_COMMANDINTERPRETER_STATE_INIT;
	COM_CommandInterpreter_Context.WritePointer = 0;
	
	COM_CommandInterpreter_Context.NextStatusOneUpdateTicks = 0;
	COM_CommandInterpreter_Context.NextStatusTwoUpdateTicks = 0;
}


static void COM_CommandInterpreter_SendStatusOneResponse(void) {
	
	uint32_t TriggerRof  = (uint32_t)(FM_DetermineRof_GetRof()*10);
	uint32_t SolenoidRof = (uint32_t)(FM_FireMode_GetSolenoidRof()*10);
	
	char Buffer[COM_COMMANDINTERPRETER_BUFFER_SIZE + 1];
	
	char BallDetected;
	
	if (ACT_Eye_GetBallDetected()) {
		BallDetected = '1';
	} else {
		BallDetected = '0';
	}
	
	Buffer[0]  = COM_COMMANDINTERPRETER_CMD_CHAR_START;
	Buffer[1]  = 'q';
	Buffer[2]  = '0' + TRG_Trigger_GetState(); 
	Buffer[3]  = BallDetected;
	Buffer[4]  = '0' + (char)((SolenoidRof % 1000) / 100);
	Buffer[5]  = '0' + (char)((SolenoidRof % 100) / 10);
	Buffer[6]  = '0' + (char)((SolenoidRof % 10) / 1);
	Buffer[7]  = '0' + (char)((TriggerRof % 1000) / 100);
	Buffer[8]  = '0' + (char)((TriggerRof % 100) / 10);
	Buffer[9]  = '0' + (char)((TriggerRof % 10) / 1);
	Buffer[10] = COM_COMMANDINTERPRETER_CMD_CHAR_END;
	
	IO_SerialUart_Write((const uint8_t *)Buffer, 11);
}


static void COM_CommandInterpreter_SendStatusTwoResponse(void) {
	
	uint32_t ShotCount   = ACT_Solenoid_GetShotCount();
	float BatteryVoltage = ACT_BatteryMonitor_GetBatteryVoltage();
	
	char Buffer[COM_COMMANDINTERPRETER_BUFFER_SIZE + 1];
	
	Buffer[0]  = COM_COMMANDINTERPRETER_CMD_CHAR_START;
	Buffer[1]  = 'r';
	Buffer[2]  = '0' + (char)((((uint32_t)(BatteryVoltage * 10)) % 100) / 10);
	Buffer[3]  = '0' + (char)((((uint32_t)(BatteryVoltage * 10)) % 10) / 1);
	Buffer[4]  = '0' + (char)((ShotCount % 1000000) / 100000);
	Buffer[5]  = '0' + (char)((ShotCount % 100000) / 10000);
	Buffer[6]  = '0' + (char)((ShotCount % 10000) / 1000);
	Buffer[7]  = '0' + (char)((ShotCount % 1000) / 100);
	Buffer[8]  = '0' + (char)((ShotCount % 100) / 10);
	Buffer[9]  = '0' + (char)((ShotCount % 10) / 1);
	Buffer[10] = COM_COMMANDINTERPRETER_CMD_CHAR_END;
	
	IO_SerialUart_Write((const uint8_t *)Buffer, 11);
}


static void COM_CommandInterpreter_SendGetResponse(char ParameterId, uint32_t Value) {
	
	char Buffer[COM_COMMANDINTERPRETER_BUFFER_SIZE + 1];
	
	Buffer[0]  = COM_COMMANDINTERPRETER_CMD_CHAR_START;
	Buffer[1]  = 'p';
	Buffer[2]  = ParameterId;
	Buffer[3]  = '0' + (char)((Value % 1000000) / 100000);
	Buffer[4]  = '0' + (char)((Value % 100000) / 10000);
	Buffer[5]  = '0' + (char)((Value % 10000) / 1000);
	Buffer[6]  = '0' + (char)((Value % 1000) / 100);
	Buffer[7]  = '0' + (char)((Value % 100) / 10);
	Buffer[8]  = '0' + (char)((Value % 10) / 1);
	Buffer[9] = COM_COMMANDINTERPRETER_CMD_CHAR_END;

	IO_SerialUart_Write((const uint8_t *)Buffer, 10);
}


static void COM_CommandInterpreter_ProcessCommandGet(void) {
	
	uint32_t ParameterIndex;
	
	// get command, 1 byte parameter
	if (COM_CommandInterpreter_Context.WritePointer == 2) {
		if (((char)COM_CommandInterpreter_Context.Buffer[1] >= 'a') && ((char)COM_CommandInterpreter_Context.Buffer[1] < 'a' + CFG_CONFIG_NUM_USER_MODIFIABLE_PARAMETER)) {
			
			ParameterIndex = (uint32_t)((char)COM_CommandInterpreter_Context.Buffer[1] - 'a');
			COM_CommandInterpreter_SendGetResponse(ParameterIndex + 'a', CFG_Config_GetUserParameter(ParameterIndex));
		}
		
		// runtime parameter after config parameter
		if (((char)COM_CommandInterpreter_Context.Buffer[1] >= 'a' + CFG_CONFIG_NUM_USER_MODIFIABLE_PARAMETER) && ((char)COM_CommandInterpreter_Context.Buffer[1] < 'a' + CFG_CONFIG_NUM_USER_MODIFIABLE_PARAMETER + COM_COMMANDINTERPRETER_NUM_RUNTIME_PARAMETER)) {
			ParameterIndex = (uint32_t)((char)COM_CommandInterpreter_Context.Buffer[1] - 'a' - CFG_CONFIG_NUM_USER_MODIFIABLE_PARAMETER);
			
			switch (ParameterIndex) {
				case COM_COMMANDINTERPRETER_RUNTIME_PARAMETER_ROF: 
							COM_CommandInterpreter_SendGetResponse(ParameterIndex + 'a', (uint32_t)(FM_DetermineRof_GetRof()*10));
							break;
			}
		}
	}
}


static void COM_CommandInterpreter_ProcessCommandSet(void) {
	
	uint32_t ParameterIndex;
	uint32_t ValueIndex;
	uint32_t Value;
	uint32_t Exponent;
	
	Value    = 0;
	Exponent = 100000;
	
	// set command, 1 byte parameter id, 6 bytes value
	if (COM_CommandInterpreter_Context.WritePointer == 8) {
		if (((char)COM_CommandInterpreter_Context.Buffer[1] >= 'a') && ((char)COM_CommandInterpreter_Context.Buffer[1] < 'a' + CFG_CONFIG_NUM_USER_MODIFIABLE_PARAMETER)) {
			
			ParameterIndex = (uint32_t)((char)COM_CommandInterpreter_Context.Buffer[1] - 'a');
			
			for (ValueIndex = 0; ValueIndex < 6; ValueIndex++) {
				if (((char)COM_CommandInterpreter_Context.Buffer[2 + ValueIndex] >= '0') && ((char)COM_CommandInterpreter_Context.Buffer[2 + ValueIndex] <= '9')) {
					
					Value = Value + Exponent * (uint32_t)((char)COM_CommandInterpreter_Context.Buffer[2 + ValueIndex] - '0');
					
				}
				Exponent = Exponent / 10;
			}
			
			CFG_Config_SetUserParameter(ParameterIndex, Value);
			COM_CommandInterpreter_SendGetResponse(ParameterIndex + 'a', CFG_Config_GetUserParameter(ParameterIndex));
			//printf("COM_CommandInterpreter_ProcessCommandSet Value=%d\r\n", Value);
			//COM_CommandInterpreter_SendGetResponse(ParameterIndex + 'a', CFG_Config_GetUserParameter(ParameterIndex));
		}
	}
}


static void COM_CommandInterpreter_ProcessCommand(void) {
	// at least one char command
	if (COM_CommandInterpreter_Context.WritePointer > 0) {
		
		switch ((char)COM_CommandInterpreter_Context.Buffer[0]) {
					case COM_COMMANDINTERPRETER_CMD_CHAR_GET:
								COM_CommandInterpreter_ProcessCommandGet();
								break;
					case COM_COMMANDINTERPRETER_CMD_CHAR_SET:
								COM_CommandInterpreter_ProcessCommandSet();
								break;								
					default:
								break;
		}
	}
}


void COM_CommandInterpreter_Update(void) {
	
	uint16_t ReceivedChar;
	uint16_t IsNewChar;
	
	IsNewChar = IO_SerialUart_ReadChar(&ReceivedChar);

	switch(COM_CommandInterpreter_Context.State) {
		case COM_COMMANDINTERPRETER_STATE_INIT:
					COM_CommandInterpreter_Context.WritePointer = 0;
					COM_CommandInterpreter_Context.State        = COM_COMMANDINTERPRETER_STATE_WAIT_FOR_START;
					
					#ifdef COM_COMMANDINTERPRETER_UPDATE_DEBUG
					printf("COM_COMMANDINTERPRETER_STATE_INIT\r\n");
					#endif
					break;

		case COM_COMMANDINTERPRETER_STATE_WAIT_FOR_START:
					if (IsNewChar) {
						if ((char)ReceivedChar == COM_COMMANDINTERPRETER_CMD_CHAR_START) {
							COM_CommandInterpreter_Context.State = COM_COMMANDINTERPRETER_STATE_WAIT_FOR_END;
						}
						
						#ifdef COM_COMMANDINTERPRETER_UPDATE_DEBUG
						printf("COM_COMMANDINTERPRETER_STATE_WAIT_FOR_START ch=%c\r\n", (char)ReceivedChar);
						#endif
					}
					break;

		case COM_COMMANDINTERPRETER_STATE_WAIT_FOR_END:
					if (IsNewChar) {		
						if ((char)ReceivedChar == COM_COMMANDINTERPRETER_CMD_CHAR_END) {
							

							// add 0 as end marker.
							COM_CommandInterpreter_Context.Buffer[COM_CommandInterpreter_Context.WritePointer] = 0;

							// process command							
							#ifdef COM_COMMANDINTERPRETER_UPDATE_DEBUG
							printf("process cmd='%s'\r\n", COM_CommandInterpreter_Context.Buffer);
							#endif
							
							COM_CommandInterpreter_ProcessCommand();
// 							COM_CommandInterpreter_Context.NextStatusOneUpdateTicks = IO_SysTickTimer_GetTicks() + IO_SYSTICKTIMER_TICKS_PER_SEC / 5;
// 							COM_CommandInterpreter_Context.NextStatusTwoUpdateTicks = IO_SysTickTimer_GetTicks() + IO_SYSTICKTIMER_TICKS_PER_SEC / 5;
							
							COM_CommandInterpreter_Context.State = COM_COMMANDINTERPRETER_STATE_INIT;
						} else {
							if (COM_CommandInterpreter_Context.WritePointer < COM_COMMANDINTERPRETER_BUFFER_SIZE) {
								COM_CommandInterpreter_Context.Buffer[COM_CommandInterpreter_Context.WritePointer] = (char)ReceivedChar;
								COM_CommandInterpreter_Context.WritePointer ++;
							} else {
								
								// an error occured. reset.
								COM_CommandInterpreter_Context.State = COM_COMMANDINTERPRETER_STATE_INIT;
							}
						}
						#ifdef COM_COMMANDINTERPRETER_UPDATE_DEBUG
						printf("COM_COMMANDINTERPRETER_STATE_WAIT_FOR_END ch=%c\r\n", (char)ReceivedChar);
						#endif
					}
					break;
	}
	
	if ((COM_Ams001_GetIsConnected()) && (true)) {
		if (IO_SysTickTimer_GetTicks() >= COM_CommandInterpreter_Context.NextStatusOneUpdateTicks) {
			COM_CommandInterpreter_Context.NextStatusOneUpdateTicks = IO_SysTickTimer_GetTicks() + IO_SYSTICKTIMER_TICKS_PER_SEC / 5;
			
			COM_CommandInterpreter_SendStatusOneResponse();	// rofs, trigger, eye
		}
		if (IO_SysTickTimer_GetTicks() >= COM_CommandInterpreter_Context.NextStatusTwoUpdateTicks) {
			COM_CommandInterpreter_Context.NextStatusTwoUpdateTicks = IO_SysTickTimer_GetTicks() + IO_SYSTICKTIMER_TICKS_PER_SEC / 2;
			
			COM_CommandInterpreter_SendStatusTwoResponse(); // volt, shotcounter
		}		
	}
}