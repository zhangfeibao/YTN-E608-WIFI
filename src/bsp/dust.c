#include "heads.h"

#define DUST_LEVEL_ONE          50
#define DUST_LEVEL_TWO          100
#define DUST_LEVEL_THREE        150
#define DUST_LEVEL_FOUR         200
#define DUST_LEVEL_FIVE         300
#define DUST_LEVEL_SIX          500

bool_t Dust_IsReady;

volatile uint16_t Dust_RTData;
uint16_t Dust_Data;
uint8_t Dust_Level;
uint8_t Dust_LevelForFan;
volatile bool_t Dust_DataValid;

static uint16_t Dust_readyCountdown;
static uint8_t Dust_levelPre;

void Dust_DeviceInit(void)
{
    Dust_readyCountdown = 1875;
    Dust_IsReady = FALSE;
    Dust_Data = 0;
}


static uint8_t Dust_getRtLevel(void)
{
    uint8_t level;

    if (Dust_Data <= DUST_LEVEL_ONE)
    {
        level = 1;
    }
    else if (Dust_Data <= DUST_LEVEL_TWO)
    {
        level = 2;
    }
    else if (Dust_Data <= DUST_LEVEL_THREE)
    {
        level = 3;
    }
    else if (Dust_Data <= DUST_LEVEL_FOUR)
    {
        level = 4;
    }
    else if (Dust_Data <= DUST_LEVEL_FIVE)
    {
        level = 5;
    }
    else
    {
        level = 6;
    }

    return level;
}


#define DUST_BUF_LEN    4
void Dust_DataUpdate(void)
{
    static uint16_t tabs[DUST_BUF_LEN];
    uint8_t i;

    if (Dust_readyCountdown)
    {
        Dust_readyCountdown --;
        if (Dust_readyCountdown == 0)
        {
            Dust_Level = Dust_getRtLevel();
            Dust_LevelForFan = Dust_Level;
            Dust_levelPre = Dust_Level;
        }
    }
    else
    {
        Dust_IsReady = TRUE;
    }

    if (Dust_DataValid)
    {
        tabs[3] = tabs[2];
        tabs[2] = tabs[1];
        tabs[1] = tabs[0];
        tabs[0] = Dust_RTData;

        Dust_DataValid = FALSE;

        Dust_Data = 0;
        for (i = 0; i < DUST_BUF_LEN;i++)
        {
            Dust_Data += tabs[i];
        }
        Dust_Data >>= 5;
    }
}

void Dust_LevelUpdate(void)
{
    static uint8_t upperHoldCntForFan;
    static uint8_t lowerHoldCntForFan;

    static lowerHoldCnt;

    uint8_t levelCur;

    if (!Dust_IsReady)
    {
        upperHoldCntForFan = 0;
        lowerHoldCntForFan = 0;

        lowerHoldCnt = 0;

        return;
    }

    /* 更新颗粒物级别 */
    levelCur = Dust_getRtLevel();

    if (levelCur > Dust_levelPre)
    {
        lowerHoldCnt = 0;

        Dust_Level = levelCur;
        Dust_levelPre = levelCur;
    }
    else if (levelCur < Dust_levelPre)
    {
        lowerHoldCnt ++;
        if (lowerHoldCnt > 104)
        {
            lowerHoldCnt = 0;

            if (Dust_Level > 1)
            {
                Dust_Level--;
            }

            Dust_levelPre = Dust_Level;
        }
    }
    else
    {
        lowerHoldCnt = 0;
    }

    /* 更新自动风俗级别 */
    if (Dust_Level > Dust_LevelForFan)
    {
        lowerHoldCntForFan = 0;

        upperHoldCntForFan ++;
        if (upperHoldCntForFan > 104)
        {
            upperHoldCntForFan = 0;

            Dust_LevelForFan++;
        }
    }
    else if (Dust_Level < Dust_LevelForFan)
    {
        upperHoldCntForFan = 0;

        lowerHoldCntForFan ++;
        if (lowerHoldCntForFan > 208)
        {
            lowerHoldCntForFan = 0;

            Dust_LevelForFan--;
        }
    }
    else
    {
        upperHoldCntForFan = 0;
        lowerHoldCntForFan = 0;
    }
}


