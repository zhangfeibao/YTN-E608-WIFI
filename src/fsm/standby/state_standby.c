#include "heads.h"

static uint8_t NHVMode_countdown;
static uint8_t wifiResetCmdDelay;

static bool_t remoteTimerChanged;

static void _keyActionDeal(void)
{
    /* 低亮按键处理 */
    if (Sys_LightLowerCountdown == 0)
    {
        Sys_LightLowerCountdown = 60;
        Key_Released = FALSE;
        return;
    }
    Sys_LightLowerCountdown = 60;

    if (!Key_Released)
    {
        return;
    }

    /* 无高压模式只响应电源按键 */
    if (Sys_NHVModeEn)
    {
        if (Key_Code != KEY_POWER)
        {
            return;
        }
    }

    /* 待机状态按键处理 */
    if (Key_Action == KEY_ACTION_RELEASING)
    {
        /* 开关机键 */
        if (Key_Code == KEY_POWER)
        {
            Buzz_Set(1, 15, 20);
            State_TransitionTo(&State_Work, TRUE, FALSE);
        }
        /* UV 按键 */
        else if (Key_Code == KEY_UV)
        {
            Buzz_Set(1, 15, 20);
            State_TransitionTo(&State_ECheck, TRUE, FALSE);
        }
        /* 预约查看模式 */
        else if (Key_Code == KEY_AION)
        {
            Buzz_Set(1, 15, 20);
            State_TransitionTo(&State_TimerLookup, TRUE, FALSE);
        }
    }
    else if (Key_Action == KEY_ACTION_SHORT)
    {
        /* 时间设定按键 */
        if (Key_Code == KEY_CLOCK)
        {
            Buzz_Set(1, 15, 20);
            State_TransitionTo(&State_ClockSetting, TRUE, FALSE);
        }
        /* 预约键进入预约设置 */
        else if (Key_Code == KEY_BOOK)
        {
            Buzz_Set(1, 15, 20);
            State_TransitionTo(&State_TimerSetting, TRUE, FALSE);
        }
        /* 预约暂停恢复 */
        else if (Key_Code == KEY_SP)
        {
            Buzz_Set(1, 15, 20);
            State_TransitionTo(&State_TimerFnSetting, TRUE, FALSE);
        }
        else if (Key_Code == (KEY_UV | KEY_AION))
        {
            /* 复位wifi模组 */
            Protocol_EnterATmode();
            wifiResetCmdDelay = 3;

            Buzz_Set(1, 15, 20);
        }
    }
    else if (Key_Action == KEY_ACTION_LONG)
    {
        /* 长按负离子键进入无高压模式 */
        if (Key_Code == KEY_AION)
        {
            Sys_NHVModeEn = (bool_t)!Sys_NHVModeEn;
            NHVMode_countdown = 60;

            Buzz_Set(1, 15, 20);
        }
        else if (Key_Code == KEY_POWER)
        {
            Sys_Reset();

            Buzz_Set(1, 15, 20);
        }
    }
}

static void _displayControl(void)
{
    if (Sys_LightLowerCountdown == 0)
    {
        Display_LedsEn.led.powerOrangeEn = TRUE;
        return;
    }

    Display_LedsEn.led.powerOrangeEn = TRUE;

    A_CIRCLE_MID;
    A_CIRCLE_VANE1;
    A_CIRCLE_VANE3;
    A_CIRCLE_VANE5;

    /* 粉尘数据显示 */
    Display_DustData(0);

    /* 实时时间显示 */
    Display_ShowRT(Sys_ClockTime.hour, Sys_ClockTime.min, FALSE, FALSE);

    if (Display_FlashOn)
    {
        A_LABEL_COL;

        /* 无高压模式，clean图标闪烁 */
        if (Sys_NHVModeEn)
        {
            A_ICON_CLEAN;
        }
    }

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
        /* 显示最近一次的开机时间段 */
        if (Sys_TimerFunEn)
        {
            if (Sys_TimerOnIndex != (-1))
            {
                A_LABEL_TIMER_ON;
                if (Sys_TimerOnIndex == 0)
                {
                    A_LABEL_T_ON_1;
                }
                else if (Sys_TimerOnIndex == 2)
                {
                    A_LABEL_T_ON_2;
                }
                else if (Sys_TimerOnIndex == 4)
                {
                    A_LABEL_T_ON_3;
                }
            }
        }
    }
}

static void _taskExe(void)
{

}


static void _timeTick(void)
{
    if (wifiResetCmdDelay)
    {
        wifiResetCmdDelay--;
        if (wifiResetCmdDelay == 0)
        {
            Protocol_ResetWifiMode();
        }
    }

    if (Time_Flag1s)
    {
        if (Sys_LightLowerCountdown)
        {
            Sys_LightLowerCountdown --;
        }

        if (NHVMode_countdown)
        {
            NHVMode_countdown--;
            if (NHVMode_countdown == 0)
            {
                Sys_NHVModeEn = FALSE;
            }
        }
    }

    /* 定时开机计算 */
    if (Time_Flag1Min || remoteTimerChanged)
    {
        remoteTimerChanged = FALSE;

        /* 遥控器定时开机计算 */
        if (Sys_RemoteTimerCtr.funEn)
        {
            if (Sys_RemoteTimerCtr.segmentIndex != TIMER_SEGMENT_NULL)
            {
                if ((Sys_TimerSegments[Sys_RemoteTimerCtr.segmentIndex][0].hour == Sys_ClockTime.hour) &&
                    (Sys_TimerSegments[Sys_RemoteTimerCtr.segmentIndex][0].min == Sys_ClockTime.min))
                {
                    Buzz_Set(1, 15, 20);
                    State_TransitionTo(&State_Work, TRUE, FALSE);
                }
            }
        }
        else
        {
            /* 手动定时开机计算 */
            if (Sys_TimerFunEn)
            {
                if (Sys_TimerOnIndex != (-1))
                {
                    if ((Timer_Points[Sys_TimerOnIndex]->bits.hour == Sys_ClockTime.hour) &&
                        (Timer_Points[Sys_TimerOnIndex]->bits.min == Sys_ClockTime.min))
                    {
                        //Sys_TimerStartedIndex = Sys_RemoteTimerCtr.segmentIndex;
                        Sys_TimerStartedIndex = Sys_TimerOnIndex;

                        Buzz_Set(1, 15, 20);
                        State_TransitionTo(&State_Work, TRUE, FALSE);
                    }
                }
            }
        }
    }
}


static void _motorCtr(void)
{
    Motor_Stop();
}


static void _irDataDeal(void)
{
    if (Sys_LightLowerCountdown == 0)
    {
        Sys_LightLowerCountdown = 60;
        return;
    }
    Sys_LightLowerCountdown = 60;

    if (IR_Code == IR_ON_OFF)
    {
        Buzz_Set(1, 15, 20);
        State_TransitionTo(&State_Work, TRUE, FALSE);
    }
    else if (IR_Code == IR_BOOK_CHAGE)
    {
        if (Sys_RemoteTimerCtr.funEn)
        {
            Sys_RemoteTimerCtr.funEn = FALSE;
        }
        else
        {
            Sys_RemoteTimerCtr.funEn = TRUE;
        }

        Sys_RemoteTimerCtr.segmentIndex = TIMER_SEGMENT_NULL;
        Buzz_Set(1, 15, 20);
    }
    else if (IR_Code == IR_BOOK_1)
    {
        if (Sys_RemoteTimerCtr.funEn == TRUE)
        {
            if (Sys_RemoteTimerCtr.segmentIndex == 0)
            {
                Sys_RemoteTimerCtr.segmentIndex = TIMER_SEGMENT_NULL;
            }
            else
            {
                Sys_RemoteTimerCtr.segmentIndex = 0;
                remoteTimerChanged = TRUE;
            }

            Buzz_Set(1, 15, 20);
        }
    }
    else if (IR_Code == IR_BOOK_2)
    {
        if (Sys_RemoteTimerCtr.funEn == TRUE)
        {
            if (Sys_RemoteTimerCtr.segmentIndex == 1)
            {
                Sys_RemoteTimerCtr.segmentIndex = TIMER_SEGMENT_NULL;
            }
            else
            {
                Sys_RemoteTimerCtr.segmentIndex = 1;
                remoteTimerChanged = TRUE;
            }
            Buzz_Set(1, 15, 20);
        }
    }
    else if (IR_Code == IR_BOOK_3)
    {
        if (Sys_RemoteTimerCtr.funEn == TRUE)
        {
            if (Sys_RemoteTimerCtr.segmentIndex == 2)
            {
                Sys_RemoteTimerCtr.segmentIndex = TIMER_SEGMENT_NULL;
            }
            else
            {
                Sys_RemoteTimerCtr.segmentIndex = 2;
                remoteTimerChanged = TRUE;
            }
            Buzz_Set(1, 15, 20);
        }
    }
    else if (IR_Code == IR_HV_NONE)
    {
        Sys_NHVModeEn = (bool_t)!Sys_NHVModeEn;
        NHVMode_countdown = 60;

        Buzz_Set(1, 15, 20);
    }
}


static void _rxDataDeal(void)
{

}

static void _enterEventDeal(void)
{
    Sys_SuperFastModeEn = FALSE;

    Sys_PowerSta = POWER_OFF;

    wifiResetCmdDelay = 0;

    Display_IsLowLight = TRUE;
    Aip1944_BrightnessLevel = BRIGHTNESS_LEVEL6;
    Sys_LightLowerCountdown = 60;

    Sys_NHVModeEn = FALSE;

    remoteTimerChanged = FALSE;
    
    Sys_TimerStartedIndex = -1;

    /* 计算最近的开机时间点 */
    if (!Sys_RemoteTimerCtr.funEn) /* 未启用遥控功能 */
    {
        if (Sys_TimerFunEn)/* 定时设置功能处于开启状态 */
        {
            Sys_UpdateTimerOnIndex();
        }
    }

    Sys_HVpgCnt = 0;
}

static void _exitEventDeal(void)
{

}


State_t code State_Standby =
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

