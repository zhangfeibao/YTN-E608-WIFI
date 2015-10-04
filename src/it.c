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

    WDTCR |= 0x20;

    if (P1 & BIT1)
    {
        /* ��ع��� */
        PCON = 0x03;
        NOP; NOP; NOP; NOP;
        NOP; NOP; NOP; NOP;

        IE3 &= (~0x10);
        IE &= (~0x08);
        IE &= (~0x10); 
    }
    else
    {
        /* �������� */
        IE3 |= 0x10;
    }
}


struct _IRFlag
{
    uint8_t _rx_edge : 1;
    uint8_t _last_time_rx : 1;
    uint8_t _rx_delay : 1;
    uint8_t _rx_run : 1;
    uint8_t _rx_ok : 1;
    uint8_t _rx_long_ok : 1;
    uint8_t _fan_fb_is_high : 1;
};

static idata struct _IRFlag IR_Flags;

#define rx_edge         IR_Flags._rx_edge
#define last_time_rx    IR_Flags._last_time_rx
#define rx_delay        IR_Flags._rx_delay
#define rx_run          IR_Flags._rx_run
#define rx_ok           IR_Flags._rx_ok
#define rx_long_ok      IR_Flags._rx_long_ok
#define fan_fb_is_high  IR_Flags._fan_fb_is_high

void INT_BIT() interrupt 22
{
    // BIT interrupt
    static idata uint8_t t_128usCnt;

    static idata uint8_t rx_mode;
    static idata uint16_t  rxCount;
    static idata uint8_t   dataCount;
    static idata uint16_t  rxData;

    /* ����ң�ؽ��� ---------------------------------------------------------- */
    if (REMOTE_RX)
    {
        last_time_rx = 1;
    }
    /* �½��� */
    else if (last_time_rx)
    {
        last_time_rx = 0;
        rx_edge = 1;

        /* ͷ����� */
        if (rx_mode == 0)
        {
            /* ���յ�ͷ�� */
            if (rxCount >= MS13_IR)
            {
                rx_mode = 1;
                dataCount = 0;
                rx_long_ok = 0;
                rxData = 0;
            }
        }
        /* 32λ���ݽ��� */
        else if (rx_mode == 1)
        {
            rxData <<= 1;
            /* һ�����ڳ���ʱ�����1.5MS���յ������롰1�� */
            if (rxCount >= MS1_5_IR)
            {
                rxData |= BIT0;
            }
            rxCount = 0;

            dataCount++;
            /* 4�ֽڣ�32λ����ȫ��������� */
            if (dataCount >= 32)
            {
                IR_Code = rxData;

                rx_mode = 2;
                rx_ok = 1;
                rx_run = 1;

                IR_Changed = TRUE;
            }
        }
        /* ׼�����¿�ʼ���� */
        else if (rx_mode == 2)
        {
            if (rxCount >= MS150_IR)
            {
                rx_mode = 0;
                rx_run = 0;
                rx_edge = 0;
            }
        }
    }

    if (rx_edge)
    {
        rxCount++;
        /* һ������ʱ�����150MS���½��� */
        if (rxCount >= MS150_IR)
        {
            rxCount = 0;
            rx_edge = 0;
            rx_mode = 0;
            rx_run = 0;
        }
    }

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