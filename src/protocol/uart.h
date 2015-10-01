#ifndef UART_H
#define UART_H

extern queueADT Rx_Queue;
extern queueADT Tx_Queue;

extern uint8_t Rx_Buf[QueueArraySize];
extern uint8_t Tx_Buf[QueueArraySize];

extern volatile uint8_t Rx_Timeout;
extern volatile bool_t Rx_QueueDataExist;

extern volatile uint8_t Tx_Timeout;
extern volatile bool_t Tx_Busy;

void Uart_MemoryInit(void);
void Uart_SendStartup(void);
void Uart_RxDatsDeal(void);

#endif
