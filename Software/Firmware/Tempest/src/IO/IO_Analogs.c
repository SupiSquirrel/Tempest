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
	
	uint16_t LastGoodVcc;
	
} IO_Analogs_Context_Type;

static IO_Analogs_Context_Type IO_Analogs_Context;



void IO_Analogs_Initialize(void) {
	struct adc_config config_adc;

	adc_get_config_defaults(&config_adc);

	config_adc.gain_factor     = ADC_GAIN_FACTOR_DIV2;
	config_adc.clock_prescaler = ADC_CLOCK_PRESCALER_DIV512;
	config_adc.reference       = ADC_REFERENCE_INTVCC1;
#ifdef ETEK5
	config_adc.positive_input  = ADC_POSITIVE_INPUT_PIN7;
#endif
#ifdef DRONE2
	config_adc.positive_input  = ADC_POSITIVE_INPUT_PIN16;
#endif
	config_adc.resolution      = ADC_RESOLUTION_10BIT;
	config_adc.freerunning     = true;

	adc_init(&(IO_Analogs_Context.adc_instance), ADC, &config_adc);

	adc_enable(&(IO_Analogs_Context.adc_instance));
	
	adc_start_conversion(&(IO_Analogs_Context.adc_instance));
	
	IO_Analogs_Context.LastGoodVcc = 0;
	
}


void IO_Analogs_Update(void) {
	uint16_t result;
	
	if (adc_read(&(IO_Analogs_Context.adc_instance), &result) != STATUS_BUSY) {
		IO_Analogs_Context.LastGoodVcc = result;
		//printf("%d   ", result);
	}
}


uint16_t IO_Analogs_GetChannel(uint32_t channel) {
	if (channel == IO_ANALOGS_CHANNEL_PUSHBUTTON_VCC9) {
		return IO_Analogs_Context.LastGoodVcc;
	}
	return 0;
}