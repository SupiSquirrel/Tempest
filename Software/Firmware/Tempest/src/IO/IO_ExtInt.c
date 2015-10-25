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
#include "IO_ExtInt.h"



void IO_ExtInt_Initialize(void) {
#ifdef ETEK5
	struct extint_chan_conf config_extint_chan;


	extint_chan_get_config_defaults(&config_extint_chan);

	// setup external interrupt for pa05 = obs rx
	// rising edge, no pullup
	config_extint_chan.gpio_pin           = PIN_PA05A_EIC_EXTINT5;
	config_extint_chan.gpio_pin_mux       = MUX_PA05A_EIC_EXTINT5;
	config_extint_chan.gpio_pin_pull      = EXTINT_PULL_NONE;
	config_extint_chan.detection_criteria = EXTINT_DETECT_RISING;

	extint_chan_set_config(IO_EXTINT_OBS_RX_EIC_LINE, &config_extint_chan);
#endif
}


bool IO_ExtInt_ChanDetected(uint32_t Channel) {
	return extint_chan_is_detected(Channel);
}


void IO_ExtInt_ClearDetected(uint32_t Channel) {
	extint_chan_clear_detected(Channel);
}