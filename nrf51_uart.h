#ifndef __NRF51_UART_H
#define __NRF51_UART_H
#include <stdint.h>
typedef void (*nrf51_uart_rcv_callback_t)(uint8_t*, uint16_t);
void nrf51_uart_setCallback(void *cb);


void nrf51_uart_init(void);
void nrf51_uart_tx(uint8_t *data, uint16_t len_data);
void nrf51_uart_setCallback(void *cb);
#endif
