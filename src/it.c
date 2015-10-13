#include "heads.h"

volatile fastBool_t SysTick_4ms;

volatile uint8_t idata SquGen_Cnt;

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

static uint8_t pgCntH;
void INT_Timer0() interrupt 13
{
    // Timer0 interrupt
    pgCntH++;
}

extern volatile bool_t Motor_SpeedUpdated;
void initAll(void);
void INT_WT() interrupt 20
{
    // Watch timer interrupt
    static uint8_t rt_05sCnt;
    static uint8_t clearMemoryDataCnt;

    /* ����ٶ�ץȡ */
    Motor_CurSpeed = (pgCntH << 8) + T0CNT;
    T0CR |= 0x01;
    pgCntH = 0;

    if (Motor_CurSpeed == 0)
    {
        Motor_IsRunning = FALSE;
    }
    else if(Motor_CurSpeed > 10)
    {
        Motor_IsRunning = TRUE;
    }

    Motor_SpeedUpdated = TRUE;

    /* ��ת��˸��־ */
    Display_FlashOn = !Display_FlashOn;

    /* 1s��־ */
    rt_05sCnt++;
    if (rt_05sCnt >= 2)
    {
        rt_05sCnt = 0;

        Time_Flag1s++;

        Time_RTCSec++;
        if (Time_RTCSec >= 60)
        {
            Time_RTCSec = 0;

            Time_Flag1Min = TRUE;

            Sys_ClockTime.min++;
            if (Sys_ClockTime.min >= 60)
            {
                Sys_ClockTime.min = 0;

                Sys_ClockTime.hour++;
                if (Sys_ClockTime.hour >= 24)
                {
                    Sys_ClockTime.hour = 0;
                }
            }
        }
    }

    /* ���紦�� */
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

        P0 = 0;
        P1 = 0;
        P2 = 0;
        P3 = 0;
        P4 = 0;
        P5 = 0;



        clearMemoryDataCnt++;
        if (clearMemoryDataCnt > 60)
        {
            clearMemoryDataCnt = 60;
            Sys_MemoryDataExist = FALSE;
        }
    }
    else
    {
        if (clearMemoryDataCnt)
        {
            initAll();
        }

        clearMemoryDataCnt = 0;
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

#define P_CO    P23
#define P_SQU   P22


void INT_BIT() interrupt 22
{
    // BIT interrupt
    static idata uint8_t t_128usCnt;

    static idata uint8_t rx_mode;
    static idata uint16_t  rxCount;
    static idata uint8_t   dataCount;
    static idata uint16_t  rxData;

    static idata uint16_t Dust_SignalLowHoldCnt;

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

    /* ��ʪ�ȷ��� -------------------------------------------------------------*/
    SquGen_Cnt++;
    if (SquGen_Cnt <= 4)
    {
        P_CO = 0;
        P_SQU = 1;
    }
    else if (SquGen_Cnt <= 8)
    {
        P_CO = 1;
        P_SQU = 0;
        if (SquGen_Cnt == 8)
        {
            SquGen_Cnt = 0;
        }
    }

    /* �Ҷ��źż�� ----------------------------------------------------------- */
    if (P2 & BIT1)
    {
        if (Dust_SignalLowHoldCnt)
        {
            Dust_RTData = Dust_SignalLowHoldCnt;
            Dust_DataValid = TRUE;
        }
        Dust_SignalLowHoldCnt = 0;
    }
    else
    {
        Dust_SignalLowHoldCnt++;
    }


    /* ʱ���׼ ---------------------------------------------------------------*/

    t_128usCnt++;
    if (t_128usCnt >= 50)
    {
        t_128usCnt = 0;

        SysTick_4ms = TRUE;

        Display_LedDrive(TRUE);

        if (Rx_Timeout)
        {
            Rx_Timeout--;
            if (Rx_Timeout == 0)
            {
                Rx_QueueDataExist = 1;
            }
        }
    }

    if (Display_IsLowLight)
    {
        if (t_128usCnt > 8)
        {
            P3 |= (BIT1 | BIT2 | BIT3 | BIT4);
        }
    }
}