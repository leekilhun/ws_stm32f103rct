/*
 * uart.c
 *
 *  Created on: Jul 10, 2021
 *      Author: gns2l
 */



#include "uart.h"

#ifdef _USE_HW_UART

#include "qbuffer.h"
#include "cli.h"
#include "cdc.h"

#define UART_BUF_LENGTH     256
#define UART_MAX_CH         HW_UART_MAX_CH

typedef struct
{
	bool     is_open;
	uint32_t baud;
	uint8_t  rx_buf[UART_BUF_LENGTH];
	uint8_t  rx_data;
} uart_tbl_t;


#ifdef _USE_HW_CLI
static void cliUart(cli_args_t *args);
#endif

extern UART_HandleTypeDef huart1;
extern DMA_HandleTypeDef hdma_usart1_rx;

extern UART_HandleTypeDef huart2;
extern DMA_HandleTypeDef hdma_usart2_rx;

static qbuffer_t qbuffer[UART_MAX_CH];

static uart_tbl_t uart_tbl[UART_MAX_CH];

bool uartInit(void)
{
	for (int i=0; i<UART_MAX_CH; i++)
	{
		uart_tbl[i].is_open  = false;
		uart_tbl[i].baud     = 115200;
		uart_tbl[i].rx_data  = 0;
		memset(&uart_tbl[i].rx_buf[0],0,UART_BUF_LENGTH);
	}

#ifdef _USE_HW_CLI
	cliAdd("uart", cliUart);
#endif
	return true;
}

bool uartDeInit(void)
{
	return true;
}

bool uartOpen(uint8_t ch, uint32_t baud)
{
	bool ret = false;

	switch(ch)
	{
		case _DEF_UART1:
		{
			uart_tbl[ch].baud    = baud;
			uart_tbl[ch].is_open = true;
			ret = true;
		}
		break;
		case _DEF_UART2:
		{
			huart1.Init.BaudRate               = baud;
			HAL_UART_DeInit(&huart1);
			if (HAL_UART_Init(&huart1) != HAL_OK)
			{
				return false;
			}
			else
			{
				ret = true;
				uart_tbl[ch].is_open = true;
			}
			ret = true;
			uart_tbl[ch].is_open = true;
			qbufferCreate(&qbuffer[ch], &uart_tbl[ch].rx_buf[0], UART_BUF_LENGTH);
#ifdef _USE_HW_UART_1_RX_DMA
			if (huart1.RxState == HAL_UART_STATE_READY)
			{
				if(HAL_UART_Receive_DMA(&huart1, (uint8_t *)&uart_tbl[ch].rx_buf[0], UART_BUF_LENGTH) != HAL_OK)
				{
					logPrintf("HAL_UART_Receive_DMA failed! ch[%d]",_DEF_UART1);
					ret = false;
				}
				else
				{
					qbuffer[ch].in  = qbuffer[ch].len - hdma_usart1_rx.Instance->CNDTR;
					qbuffer[ch].out = qbuffer[ch].in;
				}
			}
#else
			if (HAL_UART_Receive_IT(&huart1, (uint8_t *)&uart_tbl[ch].rx_data, 1) != HAL_OK)
			{
				ret = false;
			}
#endif

		}
		break;

		case _DEF_UART3:
		{
			huart2.Init.BaudRate               = baud;
			HAL_UART_DeInit(&huart2);
			if (HAL_UART_Init(&huart2) != HAL_OK)
			{
				return false;
			}
			else
			{
				ret = true;
				uart_tbl[ch].is_open = true;
			}
			ret = true;
			uart_tbl[ch].is_open = true;
			qbufferCreate(&qbuffer[ch], &uart_tbl[ch].rx_buf[0], UART_BUF_LENGTH);
#ifdef _USE_HW_UART_2_RX_DMA
			if (huart2.RxState == HAL_UART_STATE_READY)
			{
				if(HAL_UART_Receive_DMA(&huart2, (uint8_t *)&uart_tbl[ch].rx_buf[0], UART_BUF_LENGTH) != HAL_OK)
				{
					logPrintf("HAL_UART_Receive_DMA failed! ch[%d]",_DEF_UART2);
					ret = false;
				}
				else
				{
					qbuffer[ch].in  = qbuffer[ch].len - hdma_usart2_rx.Instance->CNDTR;
					qbuffer[ch].out = qbuffer[ch].in;
				}
			}
#else
			if (HAL_UART_Receive_IT(&huart2, (uint8_t *)&uart_tbl[ch].rx_data, 1) != HAL_OK)
			{
				ret = false;
			}
#endif

		}
		break;

		default:
			break;


	}

	return ret;
}

bool uartIsOpen(uint8_t ch)
{
	bool ret = false;

	switch(ch)
	{
		case _DEF_UART1:
			ret = cdcIsConnect();
			break;
		case _DEF_UART2:
		case _DEF_UART3:
			ret = uart_tbl[ch].is_open;
			break;
		default:
			break;
	}
	return ret;
}

bool uartClose(uint8_t ch)
{
	uart_tbl[ch].is_open = false;
	return true;
}

uint32_t uartAvailable(uint8_t ch)
{
	uint32_t ret = 0;

	switch(ch)
	{
		case _DEF_UART1:
			ret = cdcAvailable();
			break;
		case _DEF_UART2:
#ifdef _USE_HW_UART_1_RX_DMA
			qbuffer[ch].in = (qbuffer[ch].len - hdma_usart1_rx.Instance->CNDTR);
#endif
			ret = qbufferAvailable(&qbuffer[ch]);
			break;
		case _DEF_UART3:
#ifdef _USE_HW_UART_2_RX_DMA
			qbuffer[ch].in = (qbuffer[ch].len - hdma_usart2_rx.Instance->CNDTR);
#endif
			ret = qbufferAvailable(&qbuffer[ch]);
			break;
		case _DEF_UART4:
			break;
		default:
			break;
	}
	return ret;
}

bool uartFlush(uint8_t ch)
{
	uint32_t pre_time;

	pre_time = millis();
	while(uartAvailable(ch))
	{
		if (millis()-pre_time >= 10)
		{
			break;
		}
		uartRead(ch);
	}

	return true;
}

uint8_t uartRead(uint8_t ch)
{
	uint8_t ret = 0;

	switch(ch)
	{
		case _DEF_UART1:
			ret = cdcRead();
			break;
		case _DEF_UART2:
		case _DEF_UART3:
			qbufferRead(&qbuffer[ch], &ret, 1);
			break;

		default:
			break;
	}

	return ret;
}

uint32_t uartWrite(uint8_t ch, uint8_t *p_data, uint32_t length)
{
	uint32_t ret = 0;
	HAL_StatusTypeDef hal_ret = HAL_OK;
	switch(ch)
	{
		case _DEF_UART1:
		{
			ret = cdcWrite(p_data, length);
		}
		break;
		case _DEF_UART2:
			hal_ret = HAL_UART_Transmit(&huart1, p_data, length, 100);
			if (hal_ret == HAL_OK)
				ret = length;
			break;
		case _DEF_UART3:
			hal_ret = HAL_UART_Transmit(&huart2, p_data, length, 100);
			if (hal_ret == HAL_OK)
				ret = length;
			break;
		default:
			break;
	}

	return ret;
}

uint32_t uartVPrintf(uint8_t ch, const char *fmt, va_list arg)
{
	uint32_t ret = 0;
	char print_buf[256];


	int len;
	len = vsnprintf(print_buf, 256, fmt, arg);

	if (len > 0)
	{
		ret = uartWrite(ch, (uint8_t *)print_buf, len);
	}

	return ret;
}

uint32_t uartPrintf(uint8_t ch, const char *fmt, ...)
{
	char buf[256];
	va_list args;
	int len;
	uint32_t ret;

	va_start(args, fmt);
	len = vsnprintf(buf, 256, fmt, args);
	ret = uartWrite(ch, (uint8_t *)buf, len);
	va_end(args);

	return ret;
}

uint32_t uartGetBaud(uint8_t ch)
{
	uint32_t ret = 0;

	switch(ch)
	{
		case _DEF_UART1:
			ret = cdcGetBaud();
			break;
		case _DEF_UART2:
		case _DEF_UART3:
			ret = uart_tbl[ch].baud;
			break;

		default:
			break;
	}

	return ret;
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

	if (huart->Instance == USART1)
	{
#ifdef _USE_HW_UART_1_RX_DMA
#else
		qbufferWrite(&qbuffer[_DEF_UART1], &uart_tbl[_DEF_UART1].rx_data, 1);
		HAL_UART_Receive_IT(&huart1, (uint8_t *)&uart_tbl[_DEF_UART1].rx_data, 1);
#endif
	}
	else if (huart->Instance == USART2)
	{
#ifdef _USE_HW_UART_2_RX_DMA
#else
		qbufferWrite(&qbuffer[_DEF_UART2], &uart_tbl[_DEF_UART2].rx_data, 1);
		HAL_UART_Receive_IT(&huart2, (uint8_t *)&uart_tbl[_DEF_UART2].rx_data, 1);
#endif
	}
}



#ifdef _USE_HW_CLI
void cliUart(cli_args_t *args)
{
	bool ret = false;


	if (args->argc == 1 && args->isStr(0, "info"))
	{
		for (int i=0; i<UART_MAX_CH; i++)
		{
			cliPrintf("_DEF_UART%d :%d bps\n", i+1,  uartGetBaud(i));
		}
		ret = true;
	}

	if (args->argc == 2 && args->isStr(0, "test"))
	{
		uint8_t uart_ch;

		uart_ch = constrain(args->getData(1), 1, UART_MAX_CH) - 1;

		if (uart_ch != cliGetPort())
		{
			uint8_t rx_data;

			while(1)
			{
				if (uartAvailable(uart_ch) > 0)
				{
					rx_data = uartRead(uart_ch);
					cliPrintf("<- _DEF_UART%d RX : 0x%X\n", uart_ch + 1, rx_data);
				}

				if (cliAvailable() > 0)
				{
					rx_data = cliRead();
					if (rx_data == 'q')
					{
						break;
					}
					else
					{
						uartWrite(uart_ch, &rx_data, 1);
						cliPrintf("-> _DEF_UART%d TX : 0x%X\n", uart_ch + 1, rx_data);
					}
				}
			}
		}
		else
		{
			cliPrintf("This is cliPort\n");
		}
		ret = true;
	}

	if (ret == false)
	{
		cliPrintf("uart info\n");
		cliPrintf("uart test ch[1~%d]\n", HW_UART_MAX_CH);
	}
}
#endif



#endif
