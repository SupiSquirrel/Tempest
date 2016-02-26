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
#include "pinmux.h"
#include "IO_Discretes.h"



void IO_Discretes_Initialize(void) {
	
	struct port_config pin_conf;
	port_get_config_defaults(&pin_conf);
	
#ifdef ETEK5
	
		// ------------ outputs
	
		// red led
		pin_conf.direction  = PORT_PIN_DIR_OUTPUT;
		port_pin_set_config(PIN_PA09, &pin_conf);
		port_pin_set_output_level(PIN_PA09, true);
	
		//green led
		pin_conf.direction  = PORT_PIN_DIR_OUTPUT;
		port_pin_set_config(PIN_PA10, &pin_conf);
		port_pin_set_output_level(PIN_PA10, true);
	
		// blue led
		pin_conf.direction  = PORT_PIN_DIR_OUTPUT;
		port_pin_set_config(PIN_PA11, &pin_conf);
		port_pin_set_output_level(PIN_PA11, true);
	
		// ams00x reset
		pin_conf.direction  = PORT_PIN_DIR_OUTPUT;
		port_pin_set_config(PIN_PA27, &pin_conf);
		port_pin_set_output_level(PIN_PA27, true);
	
		// ams00x mode_sel
		pin_conf.direction  = PORT_PIN_DIR_OUTPUT;
		port_pin_set_config(PIN_PA28, &pin_conf);
		port_pin_set_output_level(PIN_PA28, false);
	
		// solenoid
		// handled by pwm
		// 	pin_conf.direction  = PORT_PIN_DIR_OUTPUT;
		// 	port_pin_set_config(PIN_PA06, &pin_conf);
		// 	port_pin_set_output_level(PIN_PA06, false);	
	
		// obs tx
		// handled by pwm
		// 	pin_conf.direction  = PORT_PIN_DIR_OUTPUT;
		// 	port_pin_set_config(PIN_PA04, &pin_conf);
		// 	port_pin_set_output_level(PIN_PA04, false);	
	
		// power enable
		pin_conf.direction  = PORT_PIN_DIR_OUTPUT;
		port_pin_set_config(PIN_PA15, &pin_conf);
		port_pin_set_output_level(PIN_PA15, false);
	
		// debug
		pin_conf.direction  = PORT_PIN_DIR_OUTPUT;
		port_pin_set_config(PIN_PA18, &pin_conf);
		port_pin_set_output_level(PIN_PA18, false);		
	
	
		// ------------ inputs

		// ams00x status_led
		pin_conf.direction  = PORT_PIN_DIR_INPUT;
		pin_conf.input_pull = PORT_PIN_PULL_UP;
		port_pin_set_config(PIN_PA19, &pin_conf);	
	
		// tournament button
		pin_conf.direction  = PORT_PIN_DIR_INPUT;
		pin_conf.input_pull = PORT_PIN_PULL_UP;
		port_pin_set_config(PIN_PA14, &pin_conf);
	
		// trigger button
		pin_conf.direction  = PORT_PIN_DIR_INPUT;
		pin_conf.input_pull = PORT_PIN_PULL_UP;
		port_pin_set_config(PIN_PA08, &pin_conf);
	
		// main button
		pin_conf.direction  = PORT_PIN_DIR_INPUT;
		pin_conf.input_pull = PORT_PIN_PULL_NONE;
		port_pin_set_config(PIN_PA07, &pin_conf);
	
#endif
	
#ifdef DRONE2

		// ------------ outputs
	
		// red led
		pin_conf.direction  = PORT_PIN_DIR_OUTPUT;
		port_pin_set_config(PIN_PA09, &pin_conf);
		port_pin_set_output_level(PIN_PA09, true);
		system_pinmux_pin_set_output_strength(PIN_PA09, SYSTEM_PINMUX_PIN_STRENGTH_HIGH);
	
		//green led
		pin_conf.direction  = PORT_PIN_DIR_OUTPUT;
		port_pin_set_config(PIN_PA10, &pin_conf);
		port_pin_set_output_level(PIN_PA10, true);
		system_pinmux_pin_set_output_strength(PIN_PA10, SYSTEM_PINMUX_PIN_STRENGTH_HIGH);
	
		// blue led
		pin_conf.direction  = PORT_PIN_DIR_OUTPUT;
		port_pin_set_config(PIN_PA11, &pin_conf);
		port_pin_set_output_level(PIN_PA11, true);
		system_pinmux_pin_set_output_strength(PIN_PA11, SYSTEM_PINMUX_PIN_STRENGTH_HIGH);
	
		// ams00x reset
		pin_conf.direction  = PORT_PIN_DIR_OUTPUT;
		port_pin_set_config(PIN_PA27, &pin_conf);
		port_pin_set_output_level(PIN_PA27, true);
	
		// ams00x mode_sel
		pin_conf.direction  = PORT_PIN_DIR_OUTPUT;
		port_pin_set_config(PIN_PA28, &pin_conf);
		port_pin_set_output_level(PIN_PA28, false);
	
		// power enable
		pin_conf.direction  = PORT_PIN_DIR_OUTPUT;
		port_pin_set_config(PIN_PA15, &pin_conf);
		port_pin_set_output_level(PIN_PA15, false);
	
		// debug
		pin_conf.direction  = PORT_PIN_DIR_OUTPUT;
		port_pin_set_config(PIN_PA18, &pin_conf);
		port_pin_set_output_level(PIN_PA18, false);
		
		// obs tx
		// handled by gpio
		pin_conf.direction  = PORT_PIN_DIR_OUTPUT;
		port_pin_set_config(PIN_PA04, &pin_conf);
		port_pin_set_output_level(PIN_PA04, true);
		system_pinmux_pin_set_output_strength(PIN_PA04, SYSTEM_PINMUX_PIN_STRENGTH_HIGH);
	
	
		// ------------ inputs

		// ams00x status_led input
		pin_conf.direction  = PORT_PIN_DIR_INPUT;
		pin_conf.input_pull = PORT_PIN_PULL_UP;
		port_pin_set_config(PIN_PA19, &pin_conf);
	
		// tournament button
		pin_conf.direction  = PORT_PIN_DIR_INPUT;
		pin_conf.input_pull = PORT_PIN_PULL_UP;
		port_pin_set_config(PIN_PA14, &pin_conf);
	
		// trigger button
		pin_conf.direction  = PORT_PIN_DIR_INPUT;
		pin_conf.input_pull = PORT_PIN_PULL_UP;
		port_pin_set_config(PIN_PA07, &pin_conf);
	
		// main button
		pin_conf.direction  = PORT_PIN_DIR_INPUT;
		pin_conf.input_pull = PORT_PIN_PULL_NONE;
		port_pin_set_config(PIN_PA08, &pin_conf);
		
		// up button
		pin_conf.direction  = PORT_PIN_DIR_INPUT;
		pin_conf.input_pull = PORT_PIN_PULL_UP;
		port_pin_set_config(PIN_PA22, &pin_conf);
		
		// obs rx
		pin_conf.direction  = PORT_PIN_DIR_INPUT;
		pin_conf.input_pull = PORT_PIN_PULL_UP;
		port_pin_set_config(PIN_PA05, &pin_conf);		
	
#endif
	
}


bool IO_Discretes_GetInputIsSet(uint32_t channel) {
	
	switch(channel) {
		
		
#ifdef ETEK5
		case IO_DISCRETES_CHANNEL_TRIGGER:
				return (!port_pin_get_input_level(PIN_PA08));
				break;
		case IO_DISCRETES_CHANNEL_AMS_STATUSLED:
				return (port_pin_get_input_level(PIN_PA19));
				break;
		case IO_DISCRETES_CHANNEL_TOURNAMENT_SWITCH:
				return (!port_pin_get_input_level(PIN_PA14));
				break;
		case IO_DISCRETES_CHANNEL_MAIN_SWITCH:
				return (port_pin_get_input_level(PIN_PA07));
				break;	
		case IO_DISCRETES_CHANNEL_OBS_RX:
				return (port_pin_get_input_level(PIN_PA05));
				break;											
				
#endif

#ifdef DRONE2
		case IO_DISCRETES_CHANNEL_TRIGGER:
				return (!port_pin_get_input_level(PIN_PA07));
				break;
		case IO_DISCRETES_CHANNEL_AMS_STATUSLED:
				return (port_pin_get_input_level(PIN_PA19));
				break;
		case IO_DISCRETES_CHANNEL_TOURNAMENT_SWITCH:
				return (!port_pin_get_input_level(PIN_PA14));
				break;
		case IO_DISCRETES_CHANNEL_MAIN_SWITCH:
				return (port_pin_get_input_level(PIN_PA08));
				break;
		case IO_DISCRETES_CHANNEL_OBS_RX:
				return (port_pin_get_input_level(PIN_PA05));
				break;
		case IO_DISCRETES_CHANNEL_UP_SWITCH:
				return (!port_pin_get_input_level(PIN_PA22));
				break;				
		
		#endif
				
		default:
				return false;
				break;
	}
	return false;
	
}


void IO_Discretes_SetOutputActive(uint32_t channel) {
	
	switch(channel) {

#ifdef ETEK5
		case IO_DISCRETES_CHANNEL_LED_RED:
				port_pin_set_output_level(PIN_PA09, false);
				break;
		case IO_DISCRETES_CHANNEL_LED_GREEN:
				port_pin_set_output_level(PIN_PA10, false);
				break;
		case IO_DISCRETES_CHANNEL_LED_BLUE:
				port_pin_set_output_level(PIN_PA11, false);
				break;
		case IO_DISCRETES_CHANNEL_AMS_RESET:
				port_pin_set_output_level(PIN_PA27, false);
				break;
		case IO_DISCRETES_CHANNEL_AMS_MODESEL_STREAM:
				port_pin_set_output_level(PIN_PA28, true);
				break;
		case IO_DISCRETES_CHANNEL_SOLENOID:
				port_pin_set_output_level(PIN_PA06, true);
				break;
		case IO_DISCRETES_CHANNEL_OBS_TX:
				port_pin_set_output_level(PIN_PA04, true);
				break;
		case IO_DISCRETES_CHANNEL_POWER_ENABLE:
				port_pin_set_output_level(PIN_PA15, true);
				break;
#endif
#ifdef DRONE2
		case IO_DISCRETES_CHANNEL_LED_RED:
			port_pin_set_output_level(PIN_PA11, false);
			break;
		case IO_DISCRETES_CHANNEL_LED_GREEN:
			port_pin_set_output_level(PIN_PA10, false);
			break;
		case IO_DISCRETES_CHANNEL_LED_BLUE:
			port_pin_set_output_level(PIN_PA09, false);
			break;
		case IO_DISCRETES_CHANNEL_AMS_RESET:
			port_pin_set_output_level(PIN_PA27, false);
			break;
		case IO_DISCRETES_CHANNEL_AMS_MODESEL_STREAM:
			port_pin_set_output_level(PIN_PA28, true);
			break;
		case IO_DISCRETES_CHANNEL_SOLENOID:
			port_pin_set_output_level(PIN_PA06, true);
			break;
		case IO_DISCRETES_CHANNEL_OBS_TX:
			port_pin_set_output_level(PIN_PA04, true);
			break;
		case IO_DISCRETES_CHANNEL_POWER_ENABLE:
			port_pin_set_output_level(PIN_PA15, true);
			break;
#endif				
		default:
				break;
	}
}


void IO_Discretes_SetOutputInactive(uint32_t channel) {
	
	switch(channel) {
#ifdef ETEK5		
		case IO_DISCRETES_CHANNEL_LED_RED:
				port_pin_set_output_level(PIN_PA09, true);
				break;
		case IO_DISCRETES_CHANNEL_LED_GREEN:
				port_pin_set_output_level(PIN_PA10, true);
				break;
		case IO_DISCRETES_CHANNEL_LED_BLUE:
				port_pin_set_output_level(PIN_PA11, true);
				break;
		case IO_DISCRETES_CHANNEL_AMS_RESET:
				port_pin_set_output_level(PIN_PA27, true);
				break;
		case IO_DISCRETES_CHANNEL_AMS_MODESEL_STREAM:
				port_pin_set_output_level(PIN_PA28, false);
				break;
		case IO_DISCRETES_CHANNEL_SOLENOID:
				port_pin_set_output_level(PIN_PA06, false);
				break;
		case IO_DISCRETES_CHANNEL_OBS_TX:
				port_pin_set_output_level(PIN_PA04, false);
				break;
		case IO_DISCRETES_CHANNEL_POWER_ENABLE:
				port_pin_set_output_level(PIN_PA15, false);
				break;
#endif
#ifdef DRONE2
		case IO_DISCRETES_CHANNEL_LED_RED:
			port_pin_set_output_level(PIN_PA11, true);
			break;
		case IO_DISCRETES_CHANNEL_LED_GREEN:
			port_pin_set_output_level(PIN_PA10, true);
			break;
		case IO_DISCRETES_CHANNEL_LED_BLUE:
			port_pin_set_output_level(PIN_PA09, true);
			break;
		case IO_DISCRETES_CHANNEL_AMS_RESET:
			port_pin_set_output_level(PIN_PA27, true);
			break;
		case IO_DISCRETES_CHANNEL_AMS_MODESEL_STREAM:
			port_pin_set_output_level(PIN_PA28, false);
			break;
		case IO_DISCRETES_CHANNEL_SOLENOID:
			port_pin_set_output_level(PIN_PA06, false);
			break;
		case IO_DISCRETES_CHANNEL_OBS_TX:
			port_pin_set_output_level(PIN_PA04, false);
			break;
		case IO_DISCRETES_CHANNEL_POWER_ENABLE:
			port_pin_set_output_level(PIN_PA15, false);
			break;
#endif										
		default:
				break;
	}
}