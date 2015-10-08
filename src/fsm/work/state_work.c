#include "heads.h"

static uint8_t memoryOffCounedown;

static uint8_t dustRecord10Index;
static uint8_t dustRecord30Index;

static bool_t IsAutoOffSta;
static uint16_t AutoOffCnt;

static void _keyActionDeal(void)
{
    /* ����״̬������ִֻ�и�����ʾ���� */
    if (Sys_LightLowerCountdown == 0)
    {
        Display_IsLowLight = FALSE;
        Aip1944_BrightnessLevel = BRIGHTNESS_LEVEL6;

        Sys_LightLowerCountdown = 60;
        Key_Released = FALSE;
        return;
    }

    Sys_LightLowerCountdown = 60;

    if (!Key_Released)
    {
        return;
    }

    if (Key_Action == KEY_ACTION_RELEASING)
    {
        /* ��Դ�������� */
        if (Key_Code == KEY_POWER)
        {
            State_TransitionTo(&State_Standby, TRUE, FALSE);
            Buzz_Set(1, 15, 20);
        }
    }
    /* �̰��������� */
    else if (Key_Action == KEY_ACTION_SHORT)
    {
        /* ���ٵ����� */
        if (Key_Code == KEY_SP)
        {
            AutoOffCnt = 0;
            IsAutoOffSta = FALSE;

            if (Sys_IsAutoMode)
            {
                Sys_SpOption = SP_SLEEP;
                Sys_IsAutoMode = FALSE;
            }
            else
            {
                Sys_SpOption++;
                if (Sys_SpOption > SP_HIGH)
                {
                    Sys_IsAutoMode = TRUE;
                    if (Dust_IsReady && Dust_Level == 1)
                    {
                        IsAutoOffSta = TRUE;
                    }
                }
            }

            /* һ��״̬��UV,Aion���Զ��ر� */
            if (Sys_SpOption == SP_SLEEP)
            {
                Sys_UVLedSta = UV_LED_OFF;
                Sys_AionSta = AION_OFF;
            }
            /* 2 - 5 ���Զ�����UV�� */
            else if (Sys_SpOption > SP_SLEEP)
            {
                Sys_UVLedSta = UV_LED_ON;
                Sys_AionSta = AION_ON;
            }

            Buzz_Set(1, 15, 20);
        }
        /* UV�ư��� */
        else if (Key_Code == KEY_UV)
        {
            Sys_UVLedSta = (UVLedSta_t)(!Sys_UVLedSta);

            Buzz_Set(1, 15, 20);
        }
        /* �����Ӽ� */
        else if (Key_Code == KEY_AION)
        {
            Sys_AionSta = (AionSta_t)(!Sys_AionSta);

            Buzz_Set(1, 15, 20);
        }
    }
    else if (Key_Action == KEY_ACTION_LONG)
    {
        if (Key_Code == KEY_POWER)
        {
            memoryOffCounedown = 30;
            Sys_MemoryDataExist = TRUE;
            
            Buzz_Set(1, 15, 20);
        }
    }
}

static void _displayControl(void)
{
    /* ����ػ���ʾ */
    if (memoryOffCounedown)
    {
        if (Display_FlashOn)
        {
            A_ICON_UV;
            A_ICON_AION;
            A_LABEL_AUTO;
            A_LABEL_FAN_SP;

            Display_SpLevel(SP_LOW);
            Display_Datas[0x10] |= BIT0;
        }
        return;
    }

    /* �߿���ʾ */
    Display_SetLine();

    /* Wifi ״̬����ʾ */
    if (Sys_RouteConnSta == CONNECTED)
    {
        A_ICON_WIFI;
    }

    /* �޸�ѹģʽͼ����ʾ */
    if (Sys_NHVModeEn && Display_FlashOn)
    {
        A_ICON_CLEAN;
    }

    /* PM2.5��ʾ */
    A_LABEL_PM25;
    Display_DustData(Dust_Data);
    if (Dust_IsReady)
    {
        Display_DustLevel(Dust_Level);
    }
    else
    {
        Display_DustLevel(Time_RTCSec % 6);
    }

    /* ��ʪ����ʾ */
    
    Display_ShowTemperature(Humidity_CurrentT);
    Display_ShowHumidity(Humidity_CurrentH);

    A_LABEL_H;
    A_LABEL_T;

    /* ʵʱʱ����ʾ */
    Display_ShowRT(Sys_ClockTime.hour, Sys_ClockTime.min, FALSE, FALSE);
    if (Display_FlashOn)
    {
        A_LABEL_COL;
    }

    /* ������ʾ */
    A_LABEL_FAN_SP;
    Display_SpLevel(Sys_SpOption);

    if (Sys_IsAutoMode)
    {
        A_LABEL_AUTO;
        /* ���ݵ�ǰ������ʾ�Զ����ٵ�λ */
        if (Dust_IsReady)
        {
            /* �Զ�ͣ��״̬ */
            if (IsAutoOffSta == 1)
            {
                Display_Datas[0x24] &= 0x83; /* ����ٶȵ�λ��ʾ */

                Display_Datas[0x21] &= RBIT4;   /* �����ͼ�� */
                Display_Datas[0x24] &= RBIT7;

                A_CIRCLE_MID;
                A_CIRCLE_VANE1;
                A_CIRCLE_VANE3;
                A_CIRCLE_VANE5;
            }
            else if (Dust_LevelForFan <= 2)
            {
                Display_SpLevel(SP_SLEEP);
                Display_CircleRun((DispCircleSp_t)(5 - SP_SLEEP));
            }
            else if (Dust_LevelForFan <= 3)
            {
                Display_SpLevel(SP_QUIET);
                Display_CircleRun((DispCircleSp_t)(5 - SP_QUIET));
            }
            else if (Dust_LevelForFan <= 4)
            {
                Display_SpLevel(SP_LOW);
                Display_CircleRun((DispCircleSp_t)(5 - SP_LOW));
            }
            else
            {
                Display_SpLevel(SP_MID);
                Display_CircleRun((DispCircleSp_t)(5 - SP_MID));
            }
        }
        else
        {
            Display_SpLevel(SP_LOW);
            Display_CircleRun((DispCircleSp_t)(5 - SP_LOW));
        }
    }
    else
    {
        Display_CircleRun((DispCircleSp_t)(5 - Sys_SpOption));
    }

    if (IsAutoOffSta == 0)
    {
        /* UV��ͼ����ʾ */
        if (Sys_UVLedSta == UV_LED_ON)
        {
            A_ICON_UV;
        }

        /* ������ͼ����ʾ */
        if (Sys_AionSta == AION_ON)
        {
            A_ICON_AION;
        }
    }

    /* ��ϴͼ����ʾ */
    if (Sys_EDispEn)
    {
        A_ICON_CLEAN;
    }

    /* ��ʱ������ʾ */
    if (Sys_RemoteTimerCtr.funEn)
    {
        A_LABEL_TIMER_ON;
        A_LABEL_TIMER_OFF;
        switch (Sys_RemoteTimerCtr.segmentIndex)
        {
        case 0:
            A_LABEL_T_ON_1;
            A_LABEL_T_OFF_1;
            break;
        case 1:
            A_LABEL_T_ON_2;
            A_LABEL_T_OFF_2;
            break;
        case 2:
            A_LABEL_T_ON_3;
            A_LABEL_T_OFF_3;
            break;
        case TIMER_SEGMENT_NULL:
        default:
            A_LABEL_T_ON_1;
            A_LABEL_T_OFF_1;
            A_LABEL_T_ON_2;
            A_LABEL_T_OFF_2;
            A_LABEL_T_ON_3;
            A_LABEL_T_OFF_3;
            break;
        }
    }
    else
    {
        if (Sys_TimerStartedIndex != (-1))
        {
            A_LABEL_TIMER_OFF;
            if (Sys_TimerStartedIndex == 0)
            {
                if (Timer_Points[1]->bits.enable)
                {
                    A_LABEL_T_OFF_1;
                }
            }
            else if (Sys_TimerStartedIndex == 2)
            {
                if (Timer_Points[3]->bits.enable)
                {
                    A_LABEL_T_OFF_2;
                }
            }
            else if (Sys_TimerStartedIndex == 4)
            {
                if (Timer_Points[5]->bits.enable)
                {
                    A_LABEL_T_OFF_3;
                }
            }
        }
    }

    /* �����ư���ʾ */
    if (Sys_LightLowerCountdown)
    {
        Display_LedsEn.leds = 0xff;
        Display_LedsEn.led.powerOrangeEn = FALSE;
        Display_LedsEn.led.timeOrangeEn = FALSE;
    }
    else
    {
        Display_LedsEn.led.powerBlueEn = TRUE;
    }
}

static void _taskExe(void)
{
    Sys_LoadsEn.load.dustPower = 1;

    /* �Զ�ͣ��״̬ */
    if (IsAutoOffSta == 1)
    {
        return;
    }

    if (Sys_AionSta == AION_ON)
    {
        Sys_LoadsEn.load.aion = 1;
    }
    else
    {
        Sys_LoadsEn.load.aion = 0;
    }

    if (Sys_UVLedSta == UV_LED_ON)
    {
        Sys_LoadsEn.load.uv = 1;
    }
    else
    {
        Sys_LoadsEn.load.uv = 0;
    }

    if (Sys_NHVModeEn)
    {
        Sys_LoadsEn.load.hv = 0;
    }
    else
    {
        Sys_LoadsEn.load.hv = 1;
    }
}

static uint8_t countOfSec;
static void _timeTick(void)
{
    int8_t index;

    if (memoryOffCounedown)
    {
        memoryOffCounedown--;
        if (memoryOffCounedown == 0)
        {
            State_TransitionTo(&State_Standby, TRUE, FALSE);
            Buzz_Set(1, 15, 20);
        }
    }

    if (Time_Flag1s)
    {
        /* �Զ����Զ�ͣ������ */
        if (Sys_IsAutoMode)
        {
            if (Dust_Level == 1)
            {
                AutoOffCnt++;
                if (AutoOffCnt > 600)
                {
                    IsAutoOffSta = 1;
                }
            }
            else
            {
                IsAutoOffSta = 0;
                AutoOffCnt = 0;
            }
        }
        else
        {
            IsAutoOffSta = 0;
            AutoOffCnt = 0;
        }

        /* ������ʾ��ʱ */
        if (Sys_LightLowerCountdown)
        {
            Sys_LightLowerCountdown--;
            if (Sys_LightLowerCountdown == 0)
            {
                Display_IsLowLight = TRUE;
                Aip1944_BrightnessLevel = BRIGHTNESS_LEVEL0;
            }
        }

        /* ����ʱ���ʱ */
        countOfSec++;
        if (countOfSec >= 60)
        {
            countOfSec = 0;

            Sys_WorkTime.min++;
            if (Sys_WorkTime.min >= 60)
            {
                Sys_WorkTime.min = 0;

                Sys_WorkTime.hour++;
            }
        }
    }

    if (Time_Flag1Min)
    {
        /* ��¼PM25 10�������ݵ� */
        if (dustRecord10Index == 12)
        {
            for (index = 0; index < 11; index++)
            {
                Sys_Pm25Records_10.records[index] = Sys_Pm25Records_10.records[index + 1];
            }
            Sys_Pm25Records_10.records[11] = Dust_Data;
        }
        else
        {
            for (index = dustRecord10Index; index < 12; index++)
            {
                Sys_Pm25Records_10.records[index] = Dust_Data;
            }
        }

        dustRecord10Index++;
        if (dustRecord10Index >= 12)
        {
            dustRecord10Index = 12;
        }


        /* ��¼PM25 30�������ݵ� */
        if (dustRecord30Index == 12)
        {
            for (index = 0; index < 11; index++)
            {
                Sys_Pm25Records_30.records[index] = Sys_Pm25Records_30.records[index + 1];
            }
            Sys_Pm25Records_30.records[11] = Dust_Data;
        }
        else
        {
            for (index = dustRecord30Index; index < 12; index++)
            {
                Sys_Pm25Records_30.records[index] = Dust_Data;
            }
        }

        if ((Sys_WorkTime.min % 2) == 0)
        {
            dustRecord30Index++;
        }

        if (dustRecord30Index >= 12)
        {
            dustRecord30Index = 12;
        }


        /* ��ʱ�ػ� */
        if (Sys_RemoteTimerCtr.funEn)
        {
            if (Sys_RemoteTimerCtr.segmentIndex != TIMER_SEGMENT_NULL)
            {
                if ((Sys_TimerSegments[Sys_RemoteTimerCtr.segmentIndex][1].hour == Sys_ClockTime.hour) &&
                    (Sys_TimerSegments[Sys_RemoteTimerCtr.segmentIndex][1].min == Sys_ClockTime.min))
                {
                    Buzz_Set(1, 15, 20);
                    State_TransitionTo(&State_Standby, TRUE, FALSE);
                }
            }
        }
        else
        {
            if (Sys_TimerStartedIndex != (-1))
            {
                index = Sys_TimerStartedIndex + 1;
                if (!Timer_Points[index]->bits.enable)
                {
                    index = -1;
                }

                if (index == (-1))
                {
                    /* ��Сʱ��ػ� */
                    if (Sys_WorkTime.hour == 2)
                    {
                        State_TransitionTo(&State_Standby, TRUE, FALSE);
                        Buzz_Set(1, 15, 20);
                    }
                }
                else
                {
                    if ((Timer_Points[index]->bits.hour == Sys_ClockTime.hour) &&
                        (Timer_Points[index]->bits.min == Sys_ClockTime.min))
                    {
                        Buzz_Set(1, 15, 20);
                        State_TransitionTo(&State_Standby, TRUE, FALSE);
                    }
                }
            }
        }

        if (!Sys_NHVModeEn)
        {
            Sys_EWorkedMin++;

            if (Sys_EWorkedMin >= (500 * 60))
            {
                Sys_LeftTime = 0;

                if ((Sys_EWorkedMin % (500 * 60)) == 0)
                {
                    Sys_EDispEn = TRUE;
                }
            }
            else
            {
                Sys_LeftTime = (500 * 60 - Sys_EWorkedMin)/60 + 1;
            }
        }
    }
}


static void _motorCtr(void)
{
    /* �Զ�ͣ��״̬ */
    if (IsAutoOffSta == 1)
    {
        Motor_Stop();
    }
    else
    {
        if (IsAutoOffSta)
        {
            if (Dust_IsReady)
            {
                if (Dust_LevelForFan <= 2)
                {
                    Motor_Run(600);
                }
                else if (Dust_LevelForFan <= 3)
                {
                    Motor_Run(900);
                }
                else if (Dust_LevelForFan <= 4)
                {
                    Motor_Run(1200);
                }
                else
                {
                    Motor_Run(1600);
                }
            }
            else
            {
                Motor_Run(1200);
            }
        }
        else
        {
            switch (Sys_SpOption)
            {
            case SP_SLEEP:
                Motor_Run(600);
                break;
            case SP_QUIET:
                Motor_Run(900);
                break;
            case SP_LOW:
                Motor_Run(1200);
                break;
            case SP_MID:
                Motor_Run(1600);
                break;
            case SP_HIGH:
                Motor_Run(2300);
                break;
            default:
                break;
            }
        }
    }
}


static void _irDataDeal(void)
{
    /* ����״̬������ִֻ�и�����ʾ���� */
    if (Sys_LightLowerCountdown == 0)
    {
        Display_IsLowLight = FALSE;
        Aip1944_BrightnessLevel = BRIGHTNESS_LEVEL6;

        Sys_LightLowerCountdown = 60;
        return;
    }
    Sys_LightLowerCountdown = 60;

    /* ң�ؿ��ػ��� */
    if (IR_Code == IR_ON_OFF)
    {
        Buzz_Set(1, 15, 20);
        State_TransitionTo(&State_Standby, TRUE, FALSE);
    }
    /* ң���Զ����ټ� */
    else if (IR_Code == IR_SPEED_AUTO)
    {
        AutoOffCnt = 0;
        IsAutoOffSta = 0;

        Sys_IsAutoMode = TRUE;
        if (Dust_IsReady && Dust_Level == 1)
        {
            IsAutoOffSta = 1;
        }
        Buzz_Set(1, 15, 20);
    }
    else if (IR_Code == IR_SPEED_GEAR_1)
    {
        AutoOffCnt = 0;
        IsAutoOffSta = 0;

        Sys_SpOption = SP_SLEEP;

        Sys_IsAutoMode = FALSE;

        Sys_UVLedSta = UV_LED_OFF;
        Sys_AionSta = AION_OFF;

        Buzz_Set(1, 15, 20);

    }
    else if (IR_Code == IR_SPEED_GEAR_2)
    {
        AutoOffCnt = 0;
        IsAutoOffSta = 0;

        Sys_SpOption = SP_QUIET;

        Sys_IsAutoMode = FALSE;

        Sys_UVLedSta = UV_LED_ON;
        Sys_AionSta = AION_ON;

        Buzz_Set(1, 15, 20);
    }
    else if (IR_Code == IR_SPEED_GEAR_3)
    {
        AutoOffCnt = 0;
        IsAutoOffSta = 0;

        Sys_SpOption = SP_LOW;

        Sys_IsAutoMode = FALSE;

        Sys_UVLedSta = UV_LED_ON;
        Sys_AionSta = AION_ON;

        Buzz_Set(1, 15, 20);
    }
    else if (IR_Code == IR_SPEED_GEAR_4)
    {
        AutoOffCnt = 0;
        IsAutoOffSta = 0;

        Sys_SpOption = SP_MID;

        Sys_IsAutoMode = FALSE;

        Sys_UVLedSta = UV_LED_ON;
        Sys_AionSta = AION_ON;

        Buzz_Set(1, 15, 20);
    }
    else if (IR_Code == IR_SPEED_GEAR_5)
    {
        AutoOffCnt = 0;
        IsAutoOffSta = 0;

        Sys_SpOption = SP_HIGH;

        Sys_IsAutoMode = FALSE;

        Sys_UVLedSta = UV_LED_ON;
        Sys_AionSta = AION_ON;

        Buzz_Set(1, 15, 20);
    }
    else if (IR_Code == IR_UV)
    {
        Sys_UVLedSta = (UVLedSta_t)(!Sys_UVLedSta);
        Buzz_Set(1, 15, 20);
    }
    else if (IR_Code == IR_ION)
    {
        Sys_AionSta = (AionSta_t)(!Sys_AionSta);
        Buzz_Set(1, 15, 20);
    }
}


static void _rxDataDeal(void)
{

}

static void _enterEventDeal(void)
{
    Sys_PowerSta = POWER_ON;

    Display_IsLowLight = FALSE;
    IsAutoOffSta = 0;
    memoryOffCounedown = 0; /* ����ػ�����ʱ */

    AutoOffCnt = 0;

    dustRecord10Index = 0;    /* PM25��¼10�������ݵ�index */
    dustRecord30Index = 0;    /* PM25��¼30�������ݵ�index */

    Aip1944_BrightnessLevel = BRIGHTNESS_LEVEL6;

    Sys_LightLowerCountdown = 60;
    Dust_DeviceInit();

    /* ����Ĭ�ϲ��� */
    if (!Sys_MemoryDataExist)
    {
        Sys_SpOption = SP_LOW;
        Sys_AionSta = AION_ON;
        Sys_UVLedSta = UV_LED_ON;
    }

    countOfSec = 0;

    Sys_AmountOfWipedDust = 0;

    Sys_WorkTime.hour = 0;
    Sys_WorkTime.min = 0;
}

static void _exitEventDeal(void)
{

}


State_t code State_Work =
{
    _keyActionDeal,
    _displayControl,
    _taskExe,
    _timeTick,
    _motorCtr,
    _irDataDeal,
    _rxDataDeal,

    _enterEventDeal,
    _exitEventDeal
};

