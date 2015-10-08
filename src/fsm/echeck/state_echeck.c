#include "heads.h"

static bool_t disEn;

static void _keyActionDeal(void)
{
    Time_100msCnt = 0;

    if (Key_Action == KEY_ACTION_LONG)
    {
        if (Key_Code == KEY_UV)
        {
            Buzz_Set(1, 15, 20);

            disEn = FALSE;
            Sys_EDispEn = FALSE;
        }
    }
    else if (Key_Action == KEY_ACTION_RELEASING)
    {
        if (Key_Code == KEY_UV)
        {
            Buzz_Set(1, 15, 20);
            disEn = TRUE;
        }
    }
}

static void _displayControl(void)
{
    if (disEn)
    {
        A_ICON_CLEAN;
    }

    Display_ShowNumInRTCArea(Sys_EWorkedMin / 60);
}

static void _taskExe(void)
{

}


static void _timeTick(void)
{
    /* 10s无操作，自动回退到待机 */
    Time_100msCnt++;
    if (Time_100msCnt > 104)
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
    disEn = TRUE;
}

static void _exitEventDeal(void)
{

}


State_t code State_ECheck =
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

