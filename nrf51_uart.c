#include "nrf51_uart.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "app_uart.h"
#include "nrf.h"

#define UART_TX_BUF_SIZE 256                         /**< UART TX buffer size. */
#define UART_RX_BUF_SIZE 256                         /**< UART RX buffer size. */
#define RX_PIN_NUMBER  11
#define TX_PIN_NUMBER  9

uint8_t uart_rx_buff[100];
uint16_t uart_rx_index = 0;
static nrf51_uart_rcv_callback_t nrf51_uart_rcv_callback = NULL;

static void uart_handle(app_uart_evt_t * p_event)
{
	uint8_t c;
	if (p_event->evt_type == APP_UART_DATA_READY)
	{
		app_uart_get(&c);
		uart_rx_buff[uart_rx_index++] = c;
		if (c == '\n')
		{
			nrf51_uart_rcv_callback(uart_rx_buff, uart_rx_index);
			memset(uart_rx_buff, 0, sizeof(uart_rx_buff));
			uart_rx_index = 0;
		}
	}
	if (p_event->evt_type == APP_UART_COMMUNICATION_ERROR)
	{
			APP_ERROR_HANDLER(p_event->data.error_communication);
	}
	else if (p_event->evt_type == APP_UART_FIFO_ERROR)
	{
			APP_ERROR_HANDLER(p_event->data.error_code);
	}
}

void nrf51_uart_init(void)
{
	uint32_t err_code;
	
	const app_uart_comm_params_t comm_params =
      {
          RX_PIN_NUMBER,
          TX_PIN_NUMBER,
          0,
          0,
          APP_UART_FLOW_CONTROL_DISABLED,
          false,
          UART_BAUDRATE_BAUDRATE_Baud115200
      };

    APP_UART_FIFO_INIT(&comm_params,
                         UART_RX_BUF_SIZE,
                         UART_TX_BUF_SIZE,
                         uart_handle,
                         APP_IRQ_PRIORITY_LOWEST,
                         err_code);

    APP_ERROR_CHECK(err_code);
}

void nrf51_uart_tx(uint8_t *data, uint16_t len_data)
{
	while(len_data--)
	{
		app_uart_put(*data);
		data++;
	}
}

void nrf51_uart_setCallback(void *cb)
{
	if (cb != NULL)
	{
		nrf51_uart_rcv_callback = (nrf51_uart_rcv_callback_t)cb;
	}
}
