#ifndef SYSTEM_H
#define SYSTEM_H

typedef enum
{
    DISCONNECT = 0,
    CONNECTED,
    ROUTE_UNKNOW
}ConnSta_t;


typedef enum
{
    POWER_OFF,
    POWER_ON
}PowerSta_t;

typedef struct
{
    uint8_t hour;
    uint8_t min;
}SimpleTime_t;

typedef struct
{
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t min;
}ClockTime_t;

typedef enum
{
    SP_SLEEP = 1,
    SP_QUIET,
    SP_LOW,
    SP_MID,
    SP_HIGH
}SpOptions_t;

typedef enum
{
    FM_FIXING = 1,
    FM_NORMAL_USING
}FixMode_t;

typedef enum
{
    CM_NORMAL_USING = 1,
    CM_CLEANING
}CleanMode_t;

typedef enum
{
    AION_OFF,
    AION_ON
}AionSta_t;

typedef enum
{
    UV_LED_OFF,
    UV_LED_ON
}UVLedSta_t;

typedef union
{
    uint8_t loads;
    struct
    {
        uint8_t uv : 1;
        uint8_t aion : 1;
        uint8_t hv : 1;
        uint8_t dustPower : 1;
        uint8_t resaves : 4;
    }load;
}LoadCtr_t;

typedef union
{
    uint8_t bytes[2];
    struct
    {
        uint8_t hour : 7;
        uint8_t enable : 1;
        uint8_t min : 8;
    }bits;
}TimeSet_t;

typedef struct
{
    uint8_t funEn : 1;
    uint8_t segmentIndex : 7;
}RemoteTimerCtr_t;

typedef struct
{
    uint8_t hour;
    uint8_t min;
    uint16_t records[12];
}DustRecord_t;


typedef union
{
    uint8_t bytes[20];
    struct
    {
        uint16_t sleep;
        uint16_t quiet;
        uint16_t low;
        uint16_t mid;
        uint16_t high;
        uint16_t m_sleep;
        uint16_t m_quiet;
        uint16_t m_low;
        uint16_t m_mid;
        uint16_t m_high;
    }fan;
}UsedTimeRecord_t;



extern uint8_t Sys_WifiStrength;
extern ConnSta_t Sys_RouteConnSta;
extern ConnSta_t Sys_PlanformConnSta;

extern PowerSta_t Sys_PowerSta;
extern uint16_t Sys_AmountOfWipedDust;

extern SimpleTime_t Sys_WorkTime;
extern uint16_t Sys_TotalWorkedTime;
extern uint16_t Sys_LeftTime;
extern ClockTime_t Sys_ClockTime;

extern bool_t Sys_IsAutoMode;
extern SpOptions_t Sys_SpOption;

extern FixMode_t Sys_FixMode;
extern CleanMode_t Sys_CleanMode;

extern AionSta_t Sys_AionSta;
extern UVLedSta_t Sys_UVLedSta;

extern TimeSet_t Sys_PowerOnPoint1;
extern TimeSet_t Sys_PowerOffPoint1;
extern TimeSet_t Sys_PowerOnPoint2;
extern TimeSet_t Sys_PowerOffPoint2;
extern TimeSet_t Sys_PowerOnPoint3;
extern TimeSet_t Sys_PowerOffPoint3;

extern code TimeSet_t* Timer_Points[6];
extern code SimpleTime_t Sys_TimerSegments[3][2];

extern DustRecord_t Sys_Pm25Records_10;
extern DustRecord_t Sys_Pm25Records_30;

extern UsedTimeRecord_t Sys_UsedTimeRecord;

extern bool_t Sys_TimerFunEn;
extern RemoteTimerCtr_t Sys_RemoteTimerCtr;
extern int8_t Sys_TimerOnIndex;
extern int8_t Sys_TimerStartedIndex;

extern bool_t Sys_NHVModeEn;

extern uint8_t Sys_LightLowerCountdown;

extern volatile uint8_t Time_Flag1s;
extern volatile bool_t Time_Flag1Min;
extern uint8_t Time_RTCSec;
extern uint8_t Time_100msCnt;   /* 不精确100ms定时 */

extern uint32_t Sys_EWorkedMin;
extern bool_t Sys_EDispEn;

extern bool_t Sys_MemoryDataExist;

extern LoadCtr_t Sys_LoadsEn;

void Sys_UpdateTimerOnIndex(void);
void Sys_LoadCtr(void);

void Sys_EnterStopMode(void);

#endif

