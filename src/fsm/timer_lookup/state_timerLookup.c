#include "heads.h"

#define ALL_TIMER_IS_DISABLE -1

static uint8_t timerLookupItem;

static void getNextActiveTimerIndex(void)
{
    do
    {
        timerLookupItem++;
        if (timerLookupItem >= 6)
        {
            break;
        }
    } while (Timer_Points[timerLookupItem]->bits.enable == FALSE);
}

static void _keyActionDeal(void)
{
    Time_100msCnt = 0;

    if (Sys_TimerFunEn == FALSE)
    {
        return;
    }

    if (Key_Action == KEY_ACTION_SHORT)
    {
        if (Key_Code == KEY_SP)
        {
            if (timerLookupItem == ALL_TIMER_IS_DISABLE)
            {

            }
            else
            {
                getNextActiveTimerIndex();
                if (timerLookupItem >= 6)
                {
                    timerLookupItem = -1;
                    getNextActiveTimerIndex();
                }

                Buzz_Set(1, 5, 20);
            }
        }
    }
}

static void _displayControl(void)
{
    uint8_t t1, t2;

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

    if (timerLookupItem == ALL_TIMER_IS_DISABLE)
    {
        Display_ShowNumInRTCArea(10000);
    }
    else
    {
        t1 = Timer_Points[timerLookupItem]->bits.hour;
        t2 = Timer_Points[timerLookupItem]->bits.min;

        Display_ShowRT(t1, t2, FALSE, FALSE);

        A_LABEL_COL;

        switch (timerLookupItem)
        {
        case 0:
            A_LABEL_TIMER_ON;
            if (Display_FlashOn)
            {
                A_LABEL_T_ON_1;
            }
            break;
        case 1:
            A_LABEL_TIMER_OFF;
            if (Display_FlashOn)
            {
                A_LABEL_T_OFF_1;
            }
            break;
        case 2:
            A_LABEL_TIMER_ON;
            if (Display_FlashOn)
            {
                A_LABEL_T_ON_2;
            }
            break;
        case 3:
            A_LABEL_TIMER_OFF;
            if (Display_FlashOn)
            {
                A_LABEL_T_OFF_2;
            }
            break;
        case 4:
            A_LABEL_TIMER_ON;
            if (Display_FlashOn)
            {
                A_LABEL_T_ON_3;
            }
            break;
        case 5:
            A_LABEL_TIMER_OFF;
            if (Display_FlashOn)
            {
                A_LABEL_T_OFF_3;
            }
            break;
        default:

            break;
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
    Time_100msCnt = 0;
    /* 获取第一个可获得的定时数据点 */
    timerLookupItem = -1;
    getNextActiveTimerIndex();
    if (timerLookupItem >= 6)
    {
        timerLookupItem = ALL_TIMER_IS_DISABLE;
    }
}

static void _exitEventDeal(void)
{

}


State_t code State_TimerLookup =
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


