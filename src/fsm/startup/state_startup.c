#include "heads.h"


static void _keyActionDeal(void)
{
    
}

static void _displayControl(void)
{
    //È«ÏÔÊ¾
    Display_LedsEn.leds = 0xff;
    Display_SetAll(LED_ON);
}

static void _taskExe(void)
{

}


static void _timeTick(void)
{
    Time_100msCnt++;
    if (Time_100msCnt > 20)
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

    Aip1944_BrightnessLevel = BRIGHTNESS_LEVEL6;
    Display_IsLowLight = FALSE;
}

static void _exitEventDeal(void)
{

}


State_t code State_Startup =
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

