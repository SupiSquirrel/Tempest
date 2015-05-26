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


#ifndef IO_EXTINT_H_
#define IO_EXTINT_H_


#define IO_EXTINT_OBS_RX_EIC_LINE 5


void IO_ExtInt_Initialize(void);

bool IO_ExtInt_ChanDetected(uint32_t Channel);

void IO_ExtInt_ClearDetected(uint32_t Channel);


#endif /* IO_EXTINT_H_ */