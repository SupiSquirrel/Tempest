/**
 * \file
 *
 * \brief User board initialization template
 *
 */

#include <asf.h>
#include <board.h>
#include <conf_board.h>

extern void pushbuttonHandler(uint32_t id, uint32_t mask);
extern void pa22Handler(uint32_t id, uint32_t mask);
extern void rf231IntHandler(uint32_t id, uint32_t mask);


void board_init(void)
{
	/* This function is meant to contain board-specific initialization code
	 * for, e.g., the I/O pins. The initialization can rely on application-
	 * specific board configuration, found in conf_board.h.
	 */
	
	/* Configure LED pins */
	gpio_configure_pin(LED0_GPIO, LED0_FLAGS);
	gpio_configure_pin(LED1_GPIO, LED1_FLAGS);
	
	
	
	/* Configure SPI pins */
	gpio_configure_pin(SPI_MISO_GPIO, SPI_MISO_FLAGS);
	gpio_configure_pin(SPI_MOSI_GPIO, SPI_MOSI_FLAGS);
	gpio_configure_pin(SPI_SPCK_GPIO, SPI_SPCK_FLAGS);

	/**
	 * defined as gpio
	 */
	gpio_configure_pin(SPI_NPCS0_GPIO, SPI_NPCS0_FLAGS);
	
	
	gpio_configure_pin(RF231_RST_GPIO, RF231_RST_FLAGS);
	gpio_configure_pin(RF231_SLP_GPIO, RF231_SLP_FLAGS);
	
	//gpio_configure_pin(RF231_IRQ_GPIO, RF231_IRQ_FLAGS);
	gpio_configure_pin(GPIO_RF231IRQ, GPIO_RF231IRQ_FLAGS);
	
	/* Configure RF231IRQ */
	pmc_enable_periph_clk(PIN_RF231IRQ_ID);
//	pio_set_debounce_filter(PIN_RF231IRQ_PIO, PIN_RF231IRQ_MASK, 10);
//	pio_handler_set(PIN_RF231IRQ_PIO, PIN_RF231IRQ_ID, PIN_RF231IRQ_MASK, PIN_RF231IRQ_ATTR, rf231IntHandler);	/* Interrupt on rising edge  */
	NVIC_EnableIRQ((IRQn_Type) PIN_RF231IRQ_ID);
	//pio_handler_set_priority(PIN_RF231IRQ_PIO, (IRQn_Type) PIN_RF231IRQ_ID, 0);
	pio_enable_interrupt(PIN_RF231IRQ_PIO, PIN_RF231IRQ_MASK);	
	
	
	//gpio_configure_pin(PUSHB_GPIO, PUSHB_FLAGS);
	gpio_configure_pin(GPIO_PUSHBUTTON, GPIO_PUSHBUTTON_FLAGS);
	
	/* Configure Pushbutton 1 */
	pmc_enable_periph_clk(PIN_PUSHBUTTON_ID);
	pio_set_debounce_filter(PIN_PUSHBUTTON_PIO, PIN_PUSHBUTTON_MASK, 10);
	pio_handler_set(PIN_PUSHBUTTON_PIO, PIN_PUSHBUTTON_ID, PIN_PUSHBUTTON_MASK, PIN_PUSHBUTTON_ATTR, pushbuttonHandler);	/* Interrupt on rising edge  */
	NVIC_EnableIRQ((IRQn_Type) PIN_PUSHBUTTON_ID);
	//pio_handler_set_priority(PIN_PUSHBUTTON_PIO, (IRQn_Type) PIN_PUSHBUTTON_ID, 0);
	pio_enable_interrupt(PIN_PUSHBUTTON_PIO, PIN_PUSHBUTTON_MASK);
	
	
	//gpio_configure_pin(GPIO_PA22, GPIO_PA22_FLAGS);
	//pmc_enable_periph_clk(PIN_PA22_ID);
	//pio_set_debounce_filter(PIN_PA22_PIO, PIN_PA22_MASK, 10);
	//pio_handler_set(PIN_PA22_PIO, PIN_PA22_ID, PIN_PA22_MASK, PIN_PA22_ATTR, pa22Handler);	/* Interrupt on rising edge  */
	//NVIC_EnableIRQ((IRQn_Type) PIN_PA22_ID);
	//pio_handler_set_priority(PIN_PA22_PIO, (IRQn_Type) PIN_PA22_ID, 0);
	//pio_enable_interrupt(PIN_PA22_PIO, PIN_PA22_MASK);	
	
	
	/* Configure twi */
	gpio_configure_pin(TWI0_DATA_GPIO, TWI0_DATA_FLAGS);
	gpio_configure_pin(TWI0_CLK_GPIO, TWI0_CLK_FLAGS);
	
	
	//gpio_configure_pin(PIN_IO0_PWM, PIN_IO0_PWM_FLAGS);
	//gpio_configure_pin(PIN_IO1_PWM, PIN_IO1_PWM_FLAGS);
	//gpio_configure_pin(PIN_IO2_PWM, PIN_IO2_PWM_FLAGS);
	//gpio_configure_pin(PIN_IO3_PWM, PIN_IO3_PWM_FLAGS);
	
	
	/* Configure 3v3sw pins */
	gpio_configure_pin(PIN_3V3SW, PIN_3V3SW_FLAGS);
	
	/* ir power */
	//gpio_configure_pin(PIN_IRPWR, PIN_IRPWR_FLAGS);
	
	
	// rs232
	gpio_configure_pin(PIN_RXD0, PIN_RXD0_FLAGS);
	gpio_configure_pin(PIN_TXD0, PIN_TXD0_FLAGS);
	
	
	
	// nrf8001
// 	gpio_configure_pin(PIN_NRF8001_RDY,  PIN_NRF8001_RDY_FLAGS);
// 	pmc_enable_periph_clk(PIN_NRF8001_RDY);
// 	gpio_configure_pin(PIN_NRF8001_REQ,  PIN_NRF8001_REQ_FLAGS);
// 	gpio_configure_pin(PIN_NRF8001_RES,  PIN_NRF8001_RES_FLAGS);
// 	gpio_configure_pin(PIN_NRF8001_SCK,  PIN_NRF8001_SCK_FLAGS);
// 	gpio_configure_pin(PIN_NRF8001_MOSI, PIN_NRF8001_MOSI_FLAGS);
// 	gpio_configure_pin(PIN_NRF8001_MISO, PIN_NRF8001_MISO_FLAGS);
	

}
