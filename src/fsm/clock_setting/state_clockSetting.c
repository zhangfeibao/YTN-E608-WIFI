#include "heads.h"

#define SETTING_ITEM_HOUR   0
#define SETTING_ITEM_MIN    1
#define SETTING_ITEM_OK     2

static void hourInc(void)
{
    Sys_ClockTime.hour++;
    if (Sys_ClockTime.hour >= 24)
    {
        Sys_ClockTime.hour = 0;
    }
}

static void hourDec(void)
{
    if (Sys_ClockTime.hour)
    {
        Sys_ClockTime.hour--;
    }
    else
    {
        Sys_ClockTime.hour = 23;
    }
}

static void minInc(void)
{
    Sys_ClockTime.min++;
    if (Sys_ClockTime.min >= 60)
    {
        Sys_ClockTime.min = 0;
    }
}

static void minDec(void)
{
    if (Sys_ClockTime.min)
    {
        Sys_ClockTime.min--;
    }
    else
    {
        Sys_ClockTime.min = 59;
    }
}

static uint8_t clockSettingItem;

static void _keyActionDeal(void)
{
    Time_100msCnt = 0;

    if (Key_Action == KEY_ACTION_SHORT || Key_Action == KEY_ACTION_INTERVAL)
    {
        if (Key_Code == KEY_SP)
        {
            if (Key_Action == KEY_ACTION_INTERVAL)
            {
                return;
            }
            else
            {
                clockSettingItem++;
                if (clockSettingItem > SETTING_ITEM_OK)
                {
                    clockSettingItem = SETTING_ITEM_HOUR;
                }
            }
        }
        else if (Key_Code == KEY_UV)
        {
            if (clockSettingItem == SETTING_ITEM_HOUR)
            {
                hourInc();

                Time_RTCSec = 0;
            }
            else if (clockSettingItem == SETTING_ITEM_MIN)
            {
                minInc();

                Time_RTCSec = 0;
            }
        }
        else if (Key_Code == KEY_AION)
        {
            if (clockSettingItem == SETTING_ITEM_HOUR)
            {
                hourDec();

                Time_RTCSec = 0;
            }
            else if (clockSettingItem == SETTING_ITEM_MIN)
            {
                minDec();

                Time_RTCSec = 0;
            }
        }
        else
        {
            return;
        }

        if (Key_Action == KEY_ACTION_SHORT)
        {
            Buzz_Set(1, 15, 20);
        }
    }
}

static void _displayControl(void)
{
    if (Display_FlashOn)
    {
        A_LABEL_COL;
    }
    Display_ShowRT(Sys_ClockTime.hour, Sys_ClockTime.min,
        (bool_t)(clockSettingItem == SETTING_ITEM_HOUR),
        (bool_t)(clockSettingItem == SETTING_ITEM_MIN));

}

static void _taskExe(void)
{

}


static void _timeTick(void)
{
    Time_100msCnt++;
    if (Time_100msCnt > 50)
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
    clockSettingItem = SETTING_ITEM_HOUR;
}

static void _exitEventDeal(void)
{

}


State_t code State_ClockSetting =
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

