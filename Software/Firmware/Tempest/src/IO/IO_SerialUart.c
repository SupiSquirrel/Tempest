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
#include "IO_SerialUart.h"


#define EDBG_CDC_MODULE              SERCOM1
#define EDBG_CDC_SERCOM_MUX_SETTING  USART_RX_1_TX_0_XCK_1
#define EDBG_CDC_SERCOM_PINMUX_PAD0  PINMUX_PA16C_SERCOM1_PAD0
#define EDBG_CDC_SERCOM_PINMUX_PAD1  PINMUX_PA17C_SERCOM1_PAD1
#define EDBG_CDC_SERCOM_PINMUX_PAD2  PINMUX_UNUSED
#define EDBG_CDC_SERCOM_PINMUX_PAD3  PINMUX_UNUSED
#define EDBG_CDC_SERCOM_DMAC_ID_TX   SERCOM1_DMAC_ID_TX
#define EDBG_CDC_SERCOM_DMAC_ID_RX   SERCOM1_DMAC_ID_RX

#define IO_SERIALUART_QUEUE_SIZE 256


typedef struct {
	struct usart_module UartInstance;
	
	uint8_t QueueBuffer[256];
	uint32_t QueueHead;
	uint32_t QueueTail;
} IO_SerialUart_Context_Type;

static IO_SerialUart_Context_Type IO_SerialUart_Context;



void IO_SerialUart_Initialize(void)
{

	struct usart_config ConfigUart;

	usart_get_config_defaults(&ConfigUart);

	ConfigUart.baudrate    = 115200;
	ConfigUart.mux_setting = EDBG_CDC_SERCOM_MUX_SETTING;
	ConfigUart.pinmux_pad0 = EDBG_CDC_SERCOM_PINMUX_PAD0;
	ConfigUart.pinmux_pad1 = EDBG_CDC_SERCOM_PINMUX_PAD1;
	ConfigUart.pinmux_pad2 = EDBG_CDC_SERCOM_PINMUX_PAD2;
	ConfigUart.pinmux_pad3 = EDBG_CDC_SERCOM_PINMUX_PAD3;

	while (usart_init(&IO_SerialUart_Context.UartInstance, EDBG_CDC_MODULE, &ConfigUart) != STATUS_OK) {
	}
	
	IO_SerialUart_Context.QueueHead = 0;
	IO_SerialUart_Context.QueueTail = 0;

	usart_enable(&IO_SerialUart_Context.UartInstance);
	

}

static bool IO_SerialUart_GetQueueHasElements(void);
static uint32_t IO_SerialUart_GetQueueFreeElements(void);
static bool IO_SerialUart_WriteChar(const uint16_t tx_data);
	
	

static uint32_t IO_SerialUart_GetQueueFreeElements(void) {
	if (IO_SerialUart_Context.QueueHead >= IO_SerialUart_Context.QueueTail) {
		return IO_SERIALUART_QUEUE_SIZE - (IO_SerialUart_Context.QueueHead - IO_SerialUart_Context.QueueTail);
	} else {
		return IO_SERIALUART_QUEUE_SIZE - (IO_SerialUart_Context.QueueTail - IO_SerialUart_Context.QueueHead);
	}
}

static bool IO_SerialUart_GetQueueHasElements(void) {
	return (IO_SerialUart_Context.QueueTail != IO_SerialUart_Context.QueueHead);
}


void IO_SerialUart_Update(void) {
	if (IO_SerialUart_GetQueueHasElements()) {
		if (IO_SerialUart_WriteChar(IO_SerialUart_Context.QueueBuffer[IO_SerialUart_Context.QueueTail])) {
			IO_SerialUart_Context.QueueTail ++;
			IO_SerialUart_Context.QueueTail = (IO_SerialUart_Context.QueueTail % IO_SERIALUART_QUEUE_SIZE);		
		}
	}
}


void IO_SerialUart_Write(const uint8_t *txData, uint16_t length) {
	
	uint16_t Count;
	
	if (length <= IO_SerialUart_GetQueueFreeElements()) {
		for (Count = 0; Count < length; Count++) {
			IO_SerialUart_Context.QueueBuffer[IO_SerialUart_Context.QueueHead] = txData[Count];
			
			IO_SerialUart_Context.QueueHead ++;
			IO_SerialUart_Context.QueueHead = (IO_SerialUart_Context.QueueHead % IO_SERIALUART_QUEUE_SIZE);
		}
	}
}


static bool IO_SerialUart_WriteChar(const uint16_t tx_data) {
	
	struct usart_module *const module = &IO_SerialUart_Context.UartInstance;

	/* Get a pointer to the hardware module instance */
	SercomUsart *const usart_hw = &(module->hw->USART);

	/* Check that the transmitter is enabled */
	// 	if (!(module->transmitter_enabled)) {
	// 		return STATUS_ERR_DENIED;
	// 	}

	/* Check if USART is ready for new data */
	if (!(usart_hw->INTFLAG.reg & SERCOM_USART_INTFLAG_DRE)) {
		/* Return error code */
		return false;
	}

	/* Wait until synchronization is complete */
	_usart_wait_for_sync(module);

	/* Write data to USART module */
	usart_hw->DATA.reg = tx_data;

	//while (!(usart_hw->INTFLAG.reg & SERCOM_USART_INTFLAG_TXC)) {
		/* Wait until data is sent */
	//}
	return true;
}


void IO_SerialUart_Write_Blocking(const uint8_t *txData, uint16_t length) {
	usart_write_buffer_wait(&IO_SerialUart_Context.UartInstance, txData, length);
}


uint32_t IO_SerialUart_ReadChar(uint16_t *const RxData) {
	if (usart_read_wait(&IO_SerialUart_Context.UartInstance, RxData) == STATUS_OK) {
		return 1u;
	} else {
		return 0u;
	}
}

int _write(int file, char *ptr, int len);
int _write(int file, char *ptr, int len) {

	IO_SerialUart_Write_Blocking((uint8_t *)ptr, (uint16_t)len);
	return len;
}

int _read(int file, char *ptr, int len);
int _read(int file, char *ptr, int len) {
	return 0;
}