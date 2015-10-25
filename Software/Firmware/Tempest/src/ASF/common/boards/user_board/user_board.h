/**
 * \file
 *
 * \brief User board definition template
 *
 */

 /* This file is intended to contain definitions and configuration details for
 * features and devices that are available on the board, e.g., frequency and
 * startup time for an external crystal, external memory devices, LED and USART
 * pins.
 */

#ifndef USER_BOARD_H
#define USER_BOARD_H

#include <conf_board.h>

// External oscillator settings.
// Uncomment and set correct values if external oscillator is used.

// External oscillator frequency
#define BOARD_XOSC_HZ          18432000

// External oscillator type.
//!< External clock signal
//#define BOARD_XOSC_TYPE        XOSC_TYPE_EXTERNAL
//!< 32.768 kHz resonator on TOSC
//#define BOARD_XOSC_TYPE        XOSC_TYPE_32KHZ
//!< 0.4 to 16 MHz resonator on XTALS
//#define BOARD_XOSC_TYPE        XOSC_TYPE_XTAL

/** Board oscillator settings */
#define BOARD_FREQ_SLCK_XTAL		(32768U)
#define BOARD_FREQ_SLCK_BYPASS		(32768U)
#define BOARD_FREQ_MAINCK_XTAL		(18432000U)
#define BOARD_FREQ_MAINCK_BYPASS	(18432000U)

/** Master clock frequency */
#define BOARD_MCK					CHIP_FREQ_CPU_MAX

// External oscillator startup time
#define BOARD_OSC_STARTUP_US  15625



/*! LED #0 "L" pin definition (red) */
#define LED_0_NAME      "redLed"
#define LED0_GPIO       (PIO_PB0_IDX)
#define LED0_FLAGS      (PIO_TYPE_PIO_OUTPUT_1 | PIO_DEFAULT)
#define LED0_ACTIVE_LEVEL 0

#define PIN_LED_0       {1 << 0, PIOB, ID_PIOB, PIO_OUTPUT_0, PIO_DEFAULT}
#define PIN_LED_0_MASK  (1 << 0)
#define PIN_LED_0_PIO   PIOB
#define PIN_LED_0_ID    ID_PIOB
#define PIN_LED_0_TYPE  PIO_OUTPUT_0
#define PIN_LED_0_ATTR  PIO_DEFAULT

/*! LED #1 pin definition (green) */
#define LED_1_NAME      "greenLed"
#define LED1_GPIO       (PIO_PB1_IDX)
#define LED1_FLAGS      (PIO_TYPE_PIO_OUTPUT_1 | PIO_DEFAULT)
#define LED1_ACTIVE_LEVEL 0

#define PIN_LED_1       {1 << 1, PIOB, ID_PIOB, PIO_OUTPUT_1, PIO_DEFAULT}
#define PIN_LED_1_MASK  (1 << 1)
#define PIN_LED_1_PIO   PIOB
#define PIN_LED_1_ID    ID_PIOB
#define PIN_LED_1_TYPE  PIO_OUTPUT_1
#define PIN_LED_1_ATTR  PIO_DEFAULT

#define RF231_RST_GPIO       (PIO_PA31_IDX)
#define RF231_RST_FLAGS      (PIO_TYPE_PIO_OUTPUT_1 | PIO_DEFAULT)

#define RF231_SLP_GPIO       (PIO_PA0_IDX)
#define RF231_SLP_FLAGS      (PIO_TYPE_PIO_OUTPUT_1 | PIO_DEFAULT)

//#define RF231_IRQ_GPIO       (PIO_PA1_IDX)
//#define RF231_IRQ_FLAGS      (PIO_TYPE_PIO_INPUT | PIO_DEFAULT)
#define RF231IRQ_NAME    "RF231IRQ"
#define GPIO_RF231IRQ    (PIO_PA1_IDX)
#define GPIO_RF231IRQ_FLAGS    (PIO_INPUT | /*PIO_PULLUP | PIO_DEBOUNCE |*/ PIO_IT_RISE_EDGE)

#define PIN_RF231IRQ    {PIO_PA1, PIOA, ID_PIOA, PIO_INPUT, /*PIO_PULLUP | PIO_DEBOUNCE |*/ PIO_IT_RISE_EDGE}
#define PIN_RF231IRQ_MASK PIO_PA1
#define PIN_RF231IRQ_PIO PIOA
#define PIN_RF231IRQ_ID ID_PIOA
#define PIN_RF231IRQ_TYPE PIO_INPUT
#define PIN_RF231IRQ_ATTR /* PIO_PULLUP | PIO_DEBOUNCE |*/ PIO_IT_RISE_EDGE


//#define PUSHB_GPIO           (PIO_PB13_IDX)
//#define PUSHB_FLAGS          (PIO_TYPE_PIO_INPUT | PIO_DEFAULT)
/** pushbutton definition. pull-up + debounce + interrupt on rising edge. */
#define PUSHBUTTON_NAME    "PB1"
#define GPIO_PUSHBUTTON    (PIO_PB13_IDX)
#define GPIO_PUSHBUTTON_FLAGS    (PIO_INPUT | PIO_PULLUP | PIO_DEBOUNCE | PIO_IT_RISE_EDGE)

#define PIN_PUSHBUTTON    {PIO_PB13, PIOB, ID_PIOB, PIO_INPUT, PIO_PULLUP | PIO_DEBOUNCE | PIO_IT_RISE_EDGE}
#define PIN_PUSHBUTTON_MASK PIO_PB13
#define PIN_PUSHBUTTON_PIO PIOB
#define PIN_PUSHBUTTON_ID ID_PIOB
#define PIN_PUSHBUTTON_TYPE PIO_INPUT
#define PIN_PUSHBUTTON_ATTR PIO_PULLUP | PIO_DEBOUNCE | PIO_IT_RISE_EDGE

/*
#define GPIO_PA22    		 (PIO_PA22_IDX)
#define GPIO_PA22_FLAGS      (PIO_INPUT | PIO_DEBOUNCE | PIO_IT_RISE_EDGE)

#define PIN_PA22    {PIO_PA22, PIOA, ID_PIOA, PIO_INPUT, PIO_DEBOUNCE | PIO_IT_RISE_EDGE}
#define PIN_PA22_MASK PIO_PA22
#define PIN_PA22_PIO PIOA
#define PIN_PA22_ID ID_PIOA
#define PIN_PA22_TYPE PIO_INPUT
#define PIN_PA22_ATTR PIO_DEBOUNCE | PIO_IT_RISE_EDGE
*/

/** SPI MISO pin definition. */
#define SPI_MISO_GPIO 		 (PIO_PA12_IDX)
#define SPI_MISO_FLAGS       (PIO_PERIPH_A | PIO_DEFAULT)
/** SPI MOSI pin definition. */
#define SPI_MOSI_GPIO 		 (PIO_PA13_IDX)
#define SPI_MOSI_FLAGS       (PIO_PERIPH_A | PIO_DEFAULT)
/** SPI SPCK pin definition. */
#define SPI_SPCK_GPIO 		 (PIO_PA14_IDX)
#define SPI_SPCK_FLAGS       (PIO_PERIPH_A | PIO_DEFAULT)

/** SPI chip select 0 pin definition. (Only one configuration is possible) */
#define SPI_NPCS0_GPIO            (PIO_PA11_IDX)
#define SPI_NPCS0_FLAGS           (PIO_TYPE_PIO_OUTPUT_1 | PIO_DEFAULT)


/** TWI0 pins definition */
#define TWI0_DATA_GPIO   PIO_PA3_IDX
#define TWI0_DATA_FLAGS  (PIO_PERIPH_A | PIO_DEFAULT)
#define TWI0_CLK_GPIO    PIO_PA4_IDX
#define TWI0_CLK_FLAGS   (PIO_PERIPH_A | PIO_DEFAULT)


#define PIN_3V3SW            (PIO_PA29_IDX)
#define PIN_3V3SW_FLAGS      (PIO_TYPE_PIO_OUTPUT_1 | PIO_DEFAULT)


#define CONSOLE_UART         UART0
#define CONSOLE_UART_ID      ID_UART0 

#define PIN_RXD0             (PIO_PA9_IDX)
#define PIN_RXD0_FLAGS       (PIO_PERIPH_A | PIO_DEFAULT)

#define PIN_TXD0             (PIO_PA10_IDX)
#define PIN_TXD0_FLAGS       (PIO_PERIPH_A | PIO_DEFAULT)


#define PIN_NRF8001_RDY        (PIO_PB3_IDX)
#define PIN_NRF8001_RDY_FLAGS  (PIO_INPUT | PIO_IT_RISE_EDGE)

#define PIN_NRF8001_REQ        (PIO_PA24_IDX)
#define PIN_NRF8001_REQ_FLAGS  (PIO_TYPE_PIO_OUTPUT_1 | PIO_DEFAULT)

#define PIN_NRF8001_RES        (PIO_PB2_IDX)
#define PIN_NRF8001_RES_FLAGS  (PIO_TYPE_PIO_OUTPUT_1 | PIO_DEFAULT)

#define PIN_NRF8001_SCK        (PIO_PA21_IDX)
#define PIN_NRF8001_SCK_FLAGS  (PIO_PERIPH_A | PIO_DEFAULT)

#define PIN_NRF8001_MOSI       (PIO_PA22_IDX)
#define PIN_NRF8001_MOSI_FLAGS (PIO_PERIPH_A | PIO_DEFAULT)

#define PIN_NRF8001_MISO       (PIO_PA23_IDX)
#define PIN_NRF8001_MISO_FLAGS (PIO_PERIPH_A | PIO_DEFAULT)


#endif // USER_BOARD_H
