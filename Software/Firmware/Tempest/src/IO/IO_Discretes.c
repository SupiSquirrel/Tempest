/*
 * IO_Discretes.c
 *
 * Created: 13.04.2015 20:56:44
 *  Author: Squirrel
 */ 


#include <asf.h>
#include "IO_Discretes.h"


void IO_Discretes_Initialize(void) {
	
	// Trigger input
	gpio_configure_pin(PIO_PA24_IDX,  PIO_INPUT | PIO_PULLUP);
	pmc_enable_periph_clk(PIO_PA24_IDX);
	
	// Solenoid output
	gpio_configure_pin(PIO_PA22_IDX,  PIO_TYPE_PIO_OUTPUT_1 | PIO_DEFAULT);
}

bool IO_Discretes_GetInputIsSet(uint32_t channel) {
	
	switch(channel) {
		case IO_DISCRETES_CHANNEL_TRIGGER:
				if (gpio_pin_is_low(PIO_PA24_IDX)) {
					return true;
				}
				break;
		default:
				return false;
				break;
	}
	return false;
	
}

void IO_Discretes_SetOutputActive(uint32_t channel) {
	switch(channel) {
		case IO_DISCRETES_CHANNEL_SOLENOID:
				gpio_set_pin_high(PIO_PA22_IDX);
				break;
		default:
				break;
	}
}

void IO_Discretes_SetOutputInactive(uint32_t channel) {
	switch(channel) {
		case IO_DISCRETES_CHANNEL_SOLENOID:
				gpio_set_pin_low(PIO_PA22_IDX);
				break;
		default:
				break;
	}
}