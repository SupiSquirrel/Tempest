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
#include "IO_Pwm.h"


typedef struct {
	struct tcc_module TccInstanceTcc0;
	struct tcc_module TccInstanceTcc1;
} IO_Pwm_Context_Type;

static IO_Pwm_Context_Type IO_Pwm_Context;



void IO_Pwm_Initialize(void) {
	
	
	// TCC0 for eye rx pwm
	
	/* Structure used to store the TCC configuration parameters */
	struct tcc_config ConfigTcc0;

	/* Fill the Structure with the default values */
	tcc_get_config_defaults(&ConfigTcc0, TCC0);

	/* Configure the single slope PWM waveform generation for waveform output */
	ConfigTcc0.compare.wave_generation = TCC_WAVE_GENERATION_SINGLE_SLOPE_PWM;

	/* Configure the TCC clock source and its divider value */
	ConfigTcc0.counter.clock_source    = GCLK_GENERATOR_0;
	ConfigTcc0.counter.clock_prescaler = TCC_CLOCK_PRESCALER_DIV1;

	/* Configure the value for TOP value */
	ConfigTcc0.counter.period = 0x900;
		
	/* Configure the TCC Waveform Output pins for waveform generation output */
	ConfigTcc0.pins.enable_wave_out_pin[0] = true;
	ConfigTcc0.pins.enable_wave_out_pin[1] = false;
	ConfigTcc0.pins.enable_wave_out_pin[2] = false;
	ConfigTcc0.pins.enable_wave_out_pin[3] = false;

	ConfigTcc0.pins.wave_out_pin[0] = PIN_PA04E_TCC0_WO0;

	/* Configure the Alternate function of GPIO pins for TCC functionality */
	ConfigTcc0.pins.wave_out_pin_mux[0] = MUX_PA04E_TCC0_WO0;


	ConfigTcc0.double_buffering_enabled = true;

	/* Configure the compare channel values for the duty cycle control */
	ConfigTcc0.compare.match[0] = 0x80;
		
	tcc_init(&(IO_Pwm_Context.TccInstanceTcc0), TCC0, &ConfigTcc0);
		
	tcc_enable(&(IO_Pwm_Context.TccInstanceTcc0));
		
		
	// TCC1 for solenoid timing
	
	/* Structure used to store the TCC configuration parameters */
	struct tcc_config ConfigTcc1;

	/* Fill the Structure with the default values */
	tcc_get_config_defaults(&ConfigTcc1, TCC1);
	
	/* Configure the single slope PWM waveform generation for waveform output */
	ConfigTcc1.compare.wave_generation = TCC_WAVE_GENERATION_SINGLE_SLOPE_PWM;
	
	/* Configure the TCC clock source and its divider value */
	ConfigTcc1.counter.clock_source    = GCLK_GENERATOR_0;
	ConfigTcc1.counter.clock_prescaler = TCC_CLOCK_PRESCALER_DIV8; // 1 MHz
	
	 /* Configure the value for TOP value */
	 ConfigTcc1.counter.period = 11904;
	 
	/* Configure the TCC Waveform Output pins for waveform generation output */
	ConfigTcc1.pins.enable_wave_out_pin[0] = true;
	ConfigTcc1.pins.wave_out_pin[0]        = PIN_PA06E_TCC1_WO0;

	/* Configure the alternate function of GPIO pins for TCC functionality */
	ConfigTcc1.pins.wave_out_pin_mux[0] = MUX_PA06E_TCC1_WO0;
	
	/* Configure the Match value for the compare channel 2 for LED0 ON time*/
	ConfigTcc1.compare.match[0] = 11904;
	
	/* Invert the Waveform output[6] channel to drive LED0 */
	ConfigTcc1.wave_ext.invert[0] = false;
	
	/* Enable the One shot Feature */
	ConfigTcc1.counter.oneshot = true;
	
	tcc_init(&(IO_Pwm_Context.TccInstanceTcc1), TCC1, &ConfigTcc1);
	
	// set counter value to match value to prevent unswanted shot.
	tcc_set_count_value(&(IO_Pwm_Context.TccInstanceTcc1), 11904);	
	
	tcc_enable(&(IO_Pwm_Context.TccInstanceTcc1));
		
		
}


void IO_Pwm_OneShotTcc1(void) {
	tcc_set_count_value(&(IO_Pwm_Context.TccInstanceTcc1), 0);
	tcc_restart_counter(&(IO_Pwm_Context.TccInstanceTcc1));
}