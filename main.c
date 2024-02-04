#include <stdint.h>
#include "nrf51_uart.h"
#include "app_timer.h"
#include "nrf_drv_clock.h"
#include "nrf_gpio.h"
#include "boards.h"

// ------------------ SOFT TIMER --------------------------------
// Khoi tao ID 
APP_TIMER_DEF(m_repeated_timer_id);
APP_TIMER_DEF(m_repeated_timer_id_1);
// Khoi tao Clock cho soft timer
static void lfclk_request(void)
{
    ret_code_t err_code = nrf_drv_clock_init();
    APP_ERROR_CHECK(err_code);
    nrf_drv_clock_lfclk_request(NULL);
}

// Ham handle repeat
static void repeated_timer_handler(void * p_context)
{
  nrf_gpio_pin_toggle(LED_1);
}

static void repeated_timer_handler_1(void * p_context)
{
	nrf51_uart_tx((uint8_t *)"Hello\n", strlen((const char *)"Hello\n"));
}
// Tao timer
static void create_timers(void)
{
	ret_code_t err_code;

	// Create timers
	err_code = app_timer_create(&m_repeated_timer_id,
															APP_TIMER_MODE_REPEATED,
															repeated_timer_handler);
	APP_ERROR_CHECK(err_code);
	
	err_code = app_timer_create(&m_repeated_timer_id_1,
															APP_TIMER_MODE_REPEATED,
															repeated_timer_handler_1);
	APP_ERROR_CHECK(err_code);
}
// ----------------------------------------------------

// ------------------- UART Callback --------------------------
void uart_rcv_callback(uint8_t *data, uint16_t len)
{
	nrf51_uart_tx(data, len);
}

int main(void)
{
	nrf51_uart_setCallback(uart_rcv_callback);
	nrf51_uart_init();
	
	nrf_gpio_cfg_output(LED_1);
	
	lfclk_request();
	static uint32_t APP_TIMER_BUF[CEIL_DIV(APP_TIMER_BUF_SIZE(3), sizeof(uint32_t))];
	app_timer_init(APP_TIMER_CONFIG_RTC_FREQUENCY, 4, APP_TIMER_BUF, NULL);
	create_timers();
	// Bat dau chay timer
	app_timer_start(m_repeated_timer_id, APP_TIMER_TICKS(200, APP_TIMER_CONFIG_RTC_FREQUENCY), NULL);
	app_timer_start(m_repeated_timer_id_1, APP_TIMER_TICKS(1000, APP_TIMER_CONFIG_RTC_FREQUENCY), NULL);
	
	
	while (1)
	{
		
	}
}

