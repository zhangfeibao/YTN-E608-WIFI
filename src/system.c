#include "heads.h"

uint8_t Sys_WifiStrength;
ConnSta_t Sys_RouteConnSta;
ConnSta_t Sys_PlanformConnSta;

PowerSta_t Sys_PowerSta;
uint16_t Sys_AmountOfWipedDust;

SimpleTime_t Sys_WorkTime;
uint16_t Sys_LeftTime;
ClockTime_t Sys_ClockTime;

bool_t Sys_IsAutoMode;
SpOptions_t Sys_SpOption;

FixMode_t Sys_FixMode;
CleanMode_t Sys_CleanMode;

AionSta_t Sys_AionSta;
UVLedSta_t Sys_UVLedSta;

TimeSet_t Sys_PowerOnPoint1;
TimeSet_t Sys_PowerOffPoint1;
TimeSet_t Sys_PowerOnPoint2;
TimeSet_t Sys_PowerOffPoint2;
TimeSet_t Sys_PowerOnPoint3;
TimeSet_t Sys_PowerOffPoint3;

DustRecord_t Sys_Pm25Records_10;
DustRecord_t Sys_Pm25Records_30;

UsedTimeRecord_t Sys_UsedTimeRecord;

bool_t Sys_TimerFunEn;
RemoteTimerCtr_t Sys_RemoteTimerCtr;
int8_t Sys_TimerOnIndex;
int8_t Sys_TimerStartedIndex;

bool_t Sys_NHVModeEn;

uint8_t Sys_LightLowerCountdown;


volatile uint8_t Time_Flag1s;
volatile bool_t Time_Flag1Min;
uint8_t Time_RTCSec;
uint8_t Time_100msCnt;

uint32_t Sys_EWorkedMin;
bool_t Sys_EDispEn;

bool_t Sys_MemoryDataExist;


LoadCtr_t Sys_LoadsEn;


code TimeSet_t* Timer_Points[6] =
{
    &Sys_PowerOnPoint1,&Sys_PowerOffPoint1,
    &Sys_PowerOnPoint2,&Sys_PowerOffPoint2,
    &Sys_PowerOnPoint3,&Sys_PowerOffPoint3
};

code SimpleTime_t Sys_TimerSegments[3][2] =
{
    { { 7,30 },{ 18,30 } },
    { { 17,30 },{ 22,30 } },
    { { 9,0 },{ 22,30 } },
};

void Sys_UpdateTimerOnIndex(void)
{
    uint16_t temp;
    uint8_t i;

    uint16_t t1;
    uint16_t t2;

    temp = 24 * 60;
    Sys_TimerOnIndex = -1;
    for (i = 0; i < 6; i += 2)
    {
        if (Timer_Points[i]->bits.enable)
        {
            t1 = Timer_Points[i]->bits.hour * 60 + Timer_Points[i]->bits.min;
            t2 = Sys_ClockTime.hour * 60 + Sys_ClockTime.min;

            if (t1 > t2)
            {
                if (t1 - t2 < temp)
                {
                    temp = t1 - t2;
                    Sys_TimerOnIndex = i;
                }
            }
        }
    }
}


void Sys_LoadCtr(void)
{
    Sys_LoadsEn.loads = 0;
    currentState->taskExe_handler();

    if (Sys_LoadsEn.load.uv)
    {
        P14 = 1;
    }
    else
    {
        P14 = 0;
    }

    if (Sys_LoadsEn.load.aion)
    {
        P16 = 1;
    }
    else
    {
        P16 = 0;
    }

    if (Sys_LoadsEn.load.hv)
    {
        P15 = 1;
    }
    else
    {
        P15 = 0;
    }

    if (Sys_LoadsEn.load.dustPower)
    {
        P43 = 1;
    }
    else
    {
        P43 = 0;
    }
}

void Sys_EnterStopMode(void)
{
    static uint8_t cnt;

    /* µç³Ø¹©µç */
    if (P1 & BIT1)
    {
        cnt++;
        if (cnt > 5)
        {
            cnt = 0;

            PCON = 0x03;
            NOP; NOP; NOP; NOP;
            NOP; NOP; NOP; NOP;

            IE3 &= (~0x10);
            IE &= (~0x08);
            IE &= (~0x10);
        }
    }
    else
    {
        cnt = 0;
    }
}





