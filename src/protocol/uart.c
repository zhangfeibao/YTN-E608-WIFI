#include "heads.h"

queueADT Rx_Queue;
queueADT Tx_Queue;

uint8_t Rx_Buf[QueueArraySize];
uint8_t Tx_Buf[50];

volatile uint8_t Rx_Timeout;
volatile bool_t Rx_QueueDataExist;

volatile uint8_t Tx_Timeout;
volatile bool_t Tx_Busy;

void Uart_MemoryInit(void)
{
	static queueCDT Rx_queue;
	static queueCDT Tx_queue;

	Rx_Queue = &Rx_queue;
	InitQueue(Rx_Queue);

	Tx_Queue = &Tx_queue;
	InitQueue(Tx_Queue);

	Rx_Timeout = 0;
	Rx_QueueDataExist = 0;

	Tx_Timeout = 0;
	Tx_Busy = 0;
}

void Uart_SendStartup(void)
{
	uint8_t len;

	if (!Tx_Busy)
	{
		len = QueueLen(Tx_Queue);

		if (len < 1)
		{
			return;
		}
		else if (len == 1)
		{
			USI1DR = Dequeue(Tx_Queue);
		}
		else
		{
			USI1DR = Dequeue(Tx_Queue);
			Tx_Busy = 1;
		}

		USI1CR2 |= (0x40 + 0x80);
	}
}

void Uart_RxDatsDeal(void)
{
	if (Rx_QueueDataExist)
	{
		Protocol_Task();
		Rx_QueueDataExist = 0;
	}
}
