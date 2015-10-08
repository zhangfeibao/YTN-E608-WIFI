#include "heads.h"


static void _keyActionDeal(void)
{
    Time_100msCnt = 0;

    if (Key_Action == KEY_ACTION_SHORT)
    {
        if (Key_Code == KEY_SP)
        {
            Sys_TimerFunEn = (bool_t)!Sys_TimerFunEn;
            Buzz_Set(1, 5, 20);
        }
    }
}

static void _displayControl(void)
{
    A_LABEL_TIMER_ON;
    A_LABEL_TIMER_OFF;
    if (Sys_TimerFunEn || Display_FlashOn)
    {
        A_LABEL_T_ON_1;
        A_LABEL_T_ON_2;
        A_LABEL_T_ON_3;

        A_LABEL_T_OFF_1;
        A_LABEL_T_OFF_2;
        A_LABEL_T_OFF_3;
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
}

static void _exitEventDeal(void)
{

}


State_t code State_TimerFnSetting =
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

