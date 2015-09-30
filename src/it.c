#include "heads.h"

volatile fastBool_t SysTick_4ms;
volatile uint8_t RT_1sCnt;

void INT_USI1_Rx() interrupt 3
{
    // USI1 Rx interrupt
    uint8_t len;

    Rx_Timeout = 4;

    //Enqueue(Rx_Queue, USI1DR);
    Rx_Queue->elements[Rx_Queue->tail] = USI1DR;
    Rx_Queue->tail = (Rx_Queue->tail + 1) % QueueArraySize;

    //if (QueueLen(Rx_Queue) > QueueArraySize - 20)
    len = ((QueueArraySize + Rx_Queue->tail - Rx_Queue->head) % QueueArraySize);
    if (len > (QueueArraySize - 20))
    {
        Rx_QueueDataExist = 1;
    }
}

void INT_USI1_Tx() interrupt 4
{
    // USI1 Tx interrupt

    //USI1DR = Dequeue(Tx_Queue);
    USI1DR = Tx_Queue->elements[Tx_Queue->head];
    Tx_Queue->head = (Tx_Queue->head + 1) % QueueArraySize;

    //if (QueueIsEmpty(Tx_Queue))
    if ((Tx_Queue->head) == (Tx_Queue->tail))
    {
        Tx_Busy = 0;
        USI1CR2 &= ~(0x40 + 0x80);
    }
}

void INT_WT() interrupt 20
{
    // Watch timer interrupt
    static uint8_t rt_05sCnt;

    Display_FlashOn = !Display_FlashOn;

    rt_05sCnt++;
    if (rt_05sCnt >= 2)
    {
        rt_05sCnt = 0;

        RT_1sCnt++;
    }

    if (P1 & BIT1)
    {
        /* 电池供电 */
        PCON = 0x03;
        NOP; NOP; NOP; NOP;
        NOP; NOP; NOP; NOP;

        IE3 &= (~0x10);
        IE &= (~0x08);
        IE &= (~0x10); 
    }
    else
    {
        /* 正常供电 */
        IE3 |= 0x10;
    }
}

void INT_BIT() interrupt 22
{
    // BIT interrupt
    static idata uint8_t t_128usCnt;

    t_128usCnt++;
    if (t_128usCnt >= 50)
    {
        t_128usCnt = 0;

        SysTick_4ms = TRUE;


        if (Rx_Timeout)
        {
            Rx_Timeout--;
            if (Rx_Timeout == 0)
            {
                Rx_QueueDataExist = 1;
            }
        }
    }
}