#include "heads.h"


static void _keyActionDeal(void)
{
    
}

static void _displayControl(void)
{
    Display_SetLine();
    A_ICON_SERVICE;
    A_ICON_AION;

    if (Sys_PlanformConnSta == CONNECTED && Sys_RouteConnSta == CONNECTED)
    {
        A_ICON_WIFI;
    }
   
    A_ICON_UV;
    A_ICON_CLEAN;
    A_CIRCLE_MID;
    A_CIRCLE_VANE1;
    A_CIRCLE_VANE2;
    A_CIRCLE_VANE3;
    A_CIRCLE_VANE4;
    A_CIRCLE_VANE5;
    A_CIRCLE_VANE6;

    A_LABEL_TIMER_ON;
    A_LABEL_TIMER_OFF;
    A_LABEL_H;
    A_LABEL_T;
    A_LABEL_FAN_SP;
    A_LABEL_AUTO;
    A_LABEL_T_ON_1;
    A_LABEL_T_ON_2;
    A_LABEL_T_ON_3;
    A_LABEL_T_OFF_1;
    A_LABEL_T_OFF_2;
    A_LABEL_T_OFF_3;


    Display_ShowTemperature(Humidity_CurrentT);
    Display_ShowHumidity(Humidity_CurrentH);
    Display_ShowRT(Sys_ClockTime.hour, Sys_ClockTime.min, FALSE, FALSE);

    if (Dust_IsReady)
    {
        //Display_DustData(Dust_Data);
        //Display_DustLevel(Dust_Level);
    }
    else
    {
        //Display_DustData(0);
    }
    Display_DustData(Sys_WifiStrength);
    
    
    A_LABEL_PM25;
    A_LABEL_COL;
}

static void _taskExe(void)
{

}


static void _timeTick(void)
{

}


static void _motorCtr(void)
{
    
}


static void _irDataDeal(void)
{
    Enqueue(Tx_Queue, (uint8_t)(IR_Code >> 8));
    Enqueue(Tx_Queue, (uint8_t)(IR_Code));

    Uart_SendStartup();

    Buzz_Set(1, 10, 15);
}


static void _rxDataDeal(void)
{
    
}

static void _enterEventDeal(void)
{
    
}

static void _exitEventDeal(void)
{

}


State_t code State_Test =
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

