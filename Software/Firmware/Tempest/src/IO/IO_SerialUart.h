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


#ifndef IO_SERIALUART_H_
#define IO_SERIALUART_H_


void IO_SerialUart_Initialize(void);
void IO_SerialUart_Update(void);
void IO_SerialUart_Write(const uint8_t *txData, uint16_t length);

void IO_SerialUart_Write_Blocking(const uint8_t *txData, uint16_t length);
uint32_t IO_SerialUart_ReadChar(uint16_t *const RxData);


#endif /* IO_SERIALUART_H_ */