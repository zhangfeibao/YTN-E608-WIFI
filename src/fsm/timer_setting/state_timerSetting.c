#include "heads.h"

#define TIMER_ON_1_H        0
#define TIMER_ON_1_M        1
#define TIMER_OFF_1_H       2
#define TIMER_OFF_1_M       3
#define TIMER_ON_2_H        4
#define TIMER_ON_2_M        5
#define TIMER_OFF_2_H       6
#define TIMER_OFF_2_M       7
#define TIMER_ON_3_H        8
#define TIMER_ON_3_M        9
#define TIMER_OFF_3_H       10
#define TIMER_OFF_3_M       11
#define TIMER_SETTING_OK    12

static uint8_t timerSettingItem;

static void _keyActionDeal(void)
{
    TimeSet_t* tsPtr;
    bool_t isM;

    Time_100msCnt = 0;

    if (Sys_TimerFunEn == FALSE)
    {
        return;
    }

    if (Key_Action == KEY_ACTION_SHORT)
    {
        tsPtr = Timer_Points[timerSettingItem / 2];
        isM = (bool_t)(timerSettingItem % 2);

        if (Key_Code == KEY_SP)
        {
            timerSettingItem++;
            if (timerSettingItem > TIMER_SETTING_OK)
            {
                timerSettingItem = TIMER_ON_1_H;
            }

            Buzz_Set(1, 15, 20);
        }
        else if (Key_Code == KEY_UV)
        {
            tsPtr->bits.enable = TRUE;

            if (isM)
            {
                tsPtr->bits.min++;
                if (tsPtr->bits.min >= 60)
                {
                    tsPtr->bits.min = 0;
                }
            }
            else
            {
                tsPtr->bits.hour++;
                if (tsPtr->bits.hour >= 24)
                {
                    tsPtr->bits.hour = 0;
                }
            }

            Buzz_Set(1, 15, 20);
        }
        else if (Key_Code == KEY_AION)
        {
            tsPtr->bits.enable = TRUE;

            if (isM)
            {
                if (tsPtr->bits.min)
                {
                    tsPtr->bits.min--;
                }
                else
                {
                    tsPtr->bits.min = 59;
                }
            }
            else
            {
                if (tsPtr->bits.hour)
                {
                    tsPtr->bits.hour--;
                }
                else
                {
                    tsPtr->bits.hour = 23;
                }
            }

            Buzz_Set(1, 15, 20);
        }
        else if (Key_Code == KEY_POWER)
        {
            if (timerSettingItem < TIMER_SETTING_OK)
            {
                if (tsPtr->bits.enable)
                {
                    tsPtr->bits.enable = FALSE;
                }
                else
                {
                    tsPtr->bits.enable = TRUE;
                }

                Buzz_Set(1, 15, 20);
            }
        }
    }
}

static void _displayControl(void)
{
    TimeSet_t* tsPtr;
    bool_t isM;

    if (Sys_TimerFunEn == FALSE)
    {
        A_LABEL_TIMER_ON;
        A_LABEL_TIMER_OFF;
        if (Display_FlashOn)
        {
            A_LABEL_T_ON_1;
            A_LABEL_T_ON_2;
            A_LABEL_T_ON_3;

            A_LABEL_T_OFF_1;
            A_LABEL_T_OFF_2;
            A_LABEL_T_OFF_3;
        }

        return;
    }

    if (timerSettingItem == TIMER_SETTING_OK)
    {
        if (Display_FlashOn)
        {
            A_LABEL_TIMER_ON;
            A_LABEL_TIMER_OFF;
        }

        Display_ShowNumInRTCArea(10000);
    }
    else
    {
        tsPtr = Timer_Points[timerSettingItem / 2];
        isM = (bool_t)(timerSettingItem % 2);

        switch (timerSettingItem)
        {
        case 0:
        case 1:
            A_LABEL_TIMER_ON;
            A_LABEL_T_ON_1;
            break;
        case 2:
        case 3:
            A_LABEL_TIMER_OFF;
            A_LABEL_T_OFF_1;
            break;
        case 4:
        case 5:
            A_LABEL_TIMER_ON;
            A_LABEL_T_ON_2;
            break;
        case 6:
        case 7:
            A_LABEL_TIMER_OFF;
            A_LABEL_T_OFF_2;
            break;
        case 8:
        case 9:
            A_LABEL_TIMER_ON;
            A_LABEL_T_ON_3;
            break;
        case 10:
        case 11:
            A_LABEL_TIMER_OFF;
            A_LABEL_T_OFF_3;
            break;
        }

        if (tsPtr->bits.enable)
        {
            Display_ShowRT(tsPtr->bits.hour, tsPtr->bits.min, (bool_t)!isM, isM);
        }
        else
        {
            Display_ShowRT(100, 100, (bool_t)!isM, isM);
        }
    }
}

static void _taskExe(void)
{

}


static void _timeTick(void)
{
    Time_100msCnt++;
    if (Time_100msCnt > 52)
    {
        State_TransitionTo(&State_Standby, TRUE, FALSE);
    }
}


static void _motorCtr(void)
{
    Motor_Stop();
}


static void _irDataDeal(void)
{

}


static void _rxDataDeal(void)
{

}

static void _enterEventDeal(void)
{
    timerSettingItem = TIMER_ON_1_H;
    Time_100msCnt = 0;
}

static void _exitEventDeal(void)
{

}


State_t code State_TimerSetting =
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

