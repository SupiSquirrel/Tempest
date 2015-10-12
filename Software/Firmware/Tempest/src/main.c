/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
#include <asf.h>


#include "sysTimer.h"
#include "string.h"
#include <math.h>
#include "IO_Discretes.h"
#include "IO_SysTickTimer.h"
#include "TRG_Trigger.h"
#include "CFG_Config.h"
#include "ACT_Solenoid.h"

/** Global g_ul_ms_ticks in milliseconds since start of application */
//volatile uint32_t g_ul_ms_ticks = 0;




/**
 *  Waits for the given number of milliseconds (using the g_ul_ms_ticks generated
 *  by the SAM's microcontrollers's system tick).
 *  \param ul_dly_ticks  Delay to wait for, in milliseconds.
 */
void mdelay(uint32_t ul_dly_ticks)
{
	uint32_t ul_cur_ticks;

	ul_cur_ticks = IO_SysTickTimer_GetTicks();
	while ((IO_SysTickTimer_GetTicks() - ul_cur_ticks) < ul_dly_ticks);
}



void pushbuttonHandler(uint32_t id, uint32_t mask);

void pushbuttonHandler(uint32_t id, uint32_t mask)
{
	gpio_set_pin_high(LED1_GPIO); // led off
	if (PIN_PUSHBUTTON_ID == id && PIN_PUSHBUTTON_MASK == mask) {
		pio_disable_interrupt(PIN_PUSHBUTTON_PIO, PIN_PUSHBUTTON_MASK);
		pio_enable_interrupt(PIN_PUSHBUTTON_PIO, PIN_PUSHBUTTON_MASK);
	}
}

uint16_t result;

static SYS_Timer_t blinkTimer;
volatile uint32_t period;



static void blinkTimerHandler(SYS_Timer_t *timer)
{
	//gpio_toggle_pin(LED0_GPIO);
}

static void startBlinkTimer(void)
{
	blinkTimer.interval = 500;
	blinkTimer.mode     = SYS_TIMER_PERIODIC_MODE;
	blinkTimer.handler  = blinkTimerHandler;
	SYS_TimerStart(&blinkTimer);
}


#define TWI_SPEED         (100000ul) /* 400KHz TWI bus frequency */

void twi_master_initialize() {
	
	sysclk_enable_peripheral_clock(ID_TWI0);
	uint32_t master_clk = sysclk_get_cpu_hz();
	
	/* TWI master initialization options. */
	twi_options_t opt = {
		.master_clk = master_clk,
		.speed = TWI_SPEED,
		.chip = 1
	};
	
	twi_master_init(TWI0, &opt);
}


void TC0_Handler(void)
{
	volatile uint32_t ul_dummy;

	/* Clear status bit to acknowledge interrupt */
	ul_dummy = tc_get_status(TC0, 0);

	/* Avoid compiler warning */
	UNUSED(ul_dummy);

	/** Toggle LED state. */
	//gpio_toggle_pin(LED0_GPIO);

}

#define TC0FREQU 15000

/**
 *  Configure Timer Counter 0 to generate an interrupt every 250ms.
 */
static void configure_tc(void)
{
	uint32_t ul_div;
	uint32_t ul_tcclks;
	uint32_t ul_sysclk = sysclk_get_cpu_hz();

	/* Configure PMC */
	pmc_enable_periph_clk(ID_TC0);

	/** Configure TC for a  Hz frequency and trigger on RC compare. */
	tc_find_mck_divisor(TC0FREQU, ul_sysclk, &ul_div, &ul_tcclks, ul_sysclk);
	tc_init(TC0, 0, ul_tcclks | TC_CMR_CPCTRG);
	tc_write_rc(TC0, 0, (ul_sysclk / ul_div) / TC0FREQU);

	/* Configure and enable interrupt on RC compare */
	NVIC_EnableIRQ((IRQn_Type) ID_TC0);
	tc_enable_interrupt(TC0, 0, TC_IER_CPCS);

	/** Start the counter */
	tc_start(TC0, 0);
}

static uint8_t consoleBuffer[256];
static uint8_t consoleBufferIndex;
/**
 *  Configure UART console.
 */
static void configure_console(void)
{
	const usart_serial_options_t uart_serial_options = {
		.baudrate = CONF_UART_BAUDRATE,
		.paritytype = CONF_UART_PARITY,
		.charlength = CONF_UART_CHAR_LENGTH,
		.stopbits = CONF_UART_STOP_BITS
	};

	/* Configure console UART. */
	sysclk_enable_peripheral_clock(CONSOLE_UART_ID);
	stdio_serial_init(CONF_UART, &uart_serial_options);
	
	memset(consoleBuffer, 0, sizeof(consoleBuffer));
	consoleBufferIndex = 0;
	
}



uint32_t temp;
uint32_t temp1;
bool triggerVal;

int main (void)
{
	
	wdt_disable(WDT);
	
	sysclk_init();
	board_init();
	
	IO_SysTickTimer_Initialize();
	IO_Discretes_Initialize();
	ACT_Solenoid_Initialize();
	
	TRG_Trigger_Initialize();
	
	CFG_Config_Initialize();

	twi_master_initialize();

	configure_console();
	printf("--------------\r\n");
	puts("   Tempest!\r\n");
	printf("--------------\r\n");

	
	// activate 3v3sw
	gpio_set_pin_low(PIN_3V3SW);
	

	SYS_TimerInit();
	
	gpio_set_pin_high(LED1_GPIO); // led off 


	//startBlinkTimer();
	
	temp1 = CFG_Config_GetUserSettingDwellTicks();
	//temp1 = CFG_Config_GetUserSettingMaximumRofEyesOnTicks();
	
	

	while (1) {
		//mdelay(2000);
		gpio_set_pin_low(LED1_GPIO);
		
		TRG_Trigger_Update();
		ACT_Solenoid_Update();
		
		
		if (TRG_Trigger_GetEdge() != TRG_TRIGGER_EDGE_NONE) {
			if (TRG_Trigger_GetEdge() == TRG_TRIGGER_EDGE_PULL) {
				ACT_Solenoid_FireShot();
				printf("TRG_TRIGGER_EDGE_PULL\n\r");
				
			} else {
				temp = TRG_Trigger_GetReleaseTimestampTicks() - TRG_Trigger_GetPullTimestampTicks();
				printf("TRG_TRIGGER_EDGE_RELEASE\n\r");
				printf("t=%d\n\n\r", temp);
			}
			
			TRG_Trigger_ClearEdge();
		}
		
		if ((TRG_Trigger_GetState() == TRG_TRIGGER_PULLED) && ((IO_SysTickTimer_GetTicks() - TRG_Trigger_GetPullTimestampTicks()) > 10000 )) {
			printf("Pulled 5s\n\r");
		}
		
		gpio_set_pin_high(LED1_GPIO);
		
		
		
		
		
		//usart_spi_putchar(0x83);
		//usart_getchar(BOARD_USART, &temp);
		//triggerVal = IO_Discretes_GetChannelIsSet(IO_DISCRETES_CHANNEL_TRIGGER);
		//printf("trg=%d\n\r", triggerVal);
// 		temp=IO_SysTickTimer_GetCurrectMs();
// 		printf("GetCurrectMs=%d\n\r", temp);
		
		
// 		if (IO_SysTickTimer_GetTicks() % 500 == 0) {
// 			gpio_toggle_pin(LED0_GPIO);
// 			mdelay(5);
// 		}
		
		
		//gpio_toggle_pin(LED1_GPIO);
		//SYS_TimerTaskHandler();
	}
}
