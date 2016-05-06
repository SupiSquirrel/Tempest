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
#include "IO_Analogs.h"


typedef struct {
	struct adc_module adc_instance;
	
	uint32_t ActiveChannel;
	
	uint16_t LastGoodVcc;
	uint16_t LastGoodEye;
	
} IO_Analogs_Context_Type;

static IO_Analogs_Context_Type IO_Analogs_Context;


#ifdef ETEK5
#define ADC_VCC_CHANNEL ADC_POSITIVE_INPUT_PIN7
#define ADC_EYE_CHANNEL ADC_POSITIVE_INPUT_PIN5 // todo: check
#endif
#ifdef DRONE2
#define ADC_VCC_CHANNEL ADC_POSITIVE_INPUT_PIN16
#define ADC_EYE_CHANNEL ADC_POSITIVE_INPUT_PIN5
#endif
#ifdef SHOCKER
#define ADC_VCC_CHANNEL ADC_POSITIVE_INPUT_PIN16
#define ADC_EYE_CHANNEL ADC_POSITIVE_INPUT_PIN5
#endif



void IO_Analogs_Initialize(void) {
	struct adc_config config_adc;

	adc_get_config_defaults(&config_adc);

	config_adc.gain_factor     = ADC_GAIN_FACTOR_DIV2;
	config_adc.clock_prescaler = ADC_CLOCK_PRESCALER_DIV512;
	config_adc.reference       = ADC_REFERENCE_INTVCC1;

	config_adc.positive_input  = ADC_VCC_CHANNEL;

	config_adc.resolution      = ADC_RESOLUTION_10BIT;
	config_adc.freerunning     = false;
	

	adc_init(&(IO_Analogs_Context.adc_instance), ADC, &config_adc);

	adc_enable(&(IO_Analogs_Context.adc_instance));
	
	adc_start_conversion(&(IO_Analogs_Context.adc_instance));
	
	IO_Analogs_Context.LastGoodVcc = 0;
	IO_Analogs_Context.LastGoodEye = 0;
	
	IO_Analogs_Context.ActiveChannel = ADC_VCC_CHANNEL;
	
}


void IO_Analogs_Update(void) {
	uint16_t result;

	
	
	if (adc_read(&(IO_Analogs_Context.adc_instance), &result) != STATUS_BUSY) {
		
		if (IO_Analogs_Context.ActiveChannel == ADC_VCC_CHANNEL) {
			IO_Analogs_Context.LastGoodVcc = result;
			adc_set_positive_input(&(IO_Analogs_Context.adc_instance), ADC_EYE_CHANNEL);
			IO_Analogs_Context.ActiveChannel = ADC_EYE_CHANNEL;
			adc_start_conversion(&(IO_Analogs_Context.adc_instance));
			
		} else if (IO_Analogs_Context.ActiveChannel == ADC_EYE_CHANNEL) {
			IO_Analogs_Context.LastGoodEye = result;
			adc_set_positive_input(&(IO_Analogs_Context.adc_instance), ADC_VCC_CHANNEL);
			IO_Analogs_Context.ActiveChannel = ADC_VCC_CHANNEL;
			adc_start_conversion(&(IO_Analogs_Context.adc_instance));
		}
		
	}
}


uint16_t IO_Analogs_GetChannel(uint32_t channel) {
	if (channel == IO_ANALOGS_CHANNEL_PUSHBUTTON_VCC9) {
		return IO_Analogs_Context.LastGoodVcc;
	}
	if (channel == IO_ANALOGS_CHANNEL_EYE) {
		return IO_Analogs_Context.LastGoodEye;
	}	
	return 0;
}