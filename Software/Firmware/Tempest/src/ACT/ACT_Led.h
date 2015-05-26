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


#ifndef ACT_LED_H_
#define ACT_LED_H_


#define ACT_LED_NUM_OF_STEPS   32

#define ACT_LED_RED             0
#define ACT_LED_GREEN           1
#define ACT_LED_BLUE            2


typedef struct {
   // 0-100% for every [ACT_LED_NUM_OF_STEPS] step
   uint32_t StepBrightness[ACT_LED_NUM_OF_STEPS];
} ACT_Led_StepBrightness_Type;


extern const ACT_Led_StepBrightness_Type ACT_Led_Pattern_Off;
extern const ACT_Led_StepBrightness_Type ACT_Led_Pattern_On;
extern const ACT_Led_StepBrightness_Type ACT_Led_Pattern_Blink;
extern const ACT_Led_StepBrightness_Type ACT_Led_Pattern_Flash;
extern const ACT_Led_StepBrightness_Type ACT_Led_Pattern_FastFlash;
extern const ACT_Led_StepBrightness_Type ACT_Led_Pattern_FlashLong;


void ACT_Led_Initialize(void);

void ACT_Led_Update(void);

void ACT_Led_SetStepBrightnessR(const ACT_Led_StepBrightness_Type* NewStepBrightnessR);

void ACT_Led_SetStepBrightnessG(const ACT_Led_StepBrightness_Type* NewStepBrightnessG);

void ACT_Led_SetStepBrightnessB(const ACT_Led_StepBrightness_Type* NewStepBrightnessB);

void ACT_Led_SetTimeOffsets(uint32_t LedIndex, uint32_t Value);

#endif /* ACT_LED_H_ */