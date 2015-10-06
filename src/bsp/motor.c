#include "heads.h"

volatile uint16_t Motor_CurSpeed;
uint16_t Motor_ExpectSpeed;
volatile bool_t Motor_IsRunning;
volatile bool_t Motor_SpeedUpdated;

static bool_t Motor_runEnable;
static uint16_t Motor_pwnData;

void Motor_Stop(void)
{
    Motor_runEnable = FALSE;
    Motor_ExpectSpeed = 0;
}

void Motor_Run(uint16_t rpm)
{
    Motor_runEnable = TRUE;
    Motor_ExpectSpeed = SP(rpm);
}

void Motor_Ctr(void)
{
    static uint16_t Motor_lastSpeed;

    uint16_t deltaSp;
    uint16_t deltaA;
    uint16_t e;

    currentState->motorCtr_handler();

    if ((!Motor_runEnable) || (Motor_ExpectSpeed == 0))
    {
        T1BDRH = 0;         // duty High
        T1BDRL = 0;  	    // duty Low
        Motor_pwnData = 0;
    }
    else
    {
        if (!Motor_IsRunning)
        {
            /* Motor startup */
            Motor_pwnData += 20;
            if (Motor_pwnData > 800)
            {
                Motor_pwnData = 800;
                /* Motor Error */
            }
        }
        else
        {
            /* Motor speed adjust */
            if (Motor_SpeedUpdated)
            {
                if (Motor_CurSpeed >= Motor_lastSpeed)
                {
                    deltaA = Motor_CurSpeed - Motor_lastSpeed;
                }
                else
                {
                    deltaA = Motor_lastSpeed - Motor_CurSpeed;
                }

                if (Motor_CurSpeed >= Motor_ExpectSpeed)
                {
                    deltaSp = Motor_CurSpeed - Motor_ExpectSpeed;
                    
                    e = (deltaSp << 4) / (deltaA + 1);

                    if (Motor_pwnData > e)
                    {
                        Motor_pwnData -= e;
                    }
                }
                else
                {
                    deltaSp = Motor_ExpectSpeed - Motor_CurSpeed;
                    e = (deltaSp << 4) / (deltaA + 1);
                    if (Motor_pwnData < (4000 - e))
                    {
                        Motor_pwnData += e;
                    }
                }

                Motor_SpeedUpdated = FALSE;
            }
        }

        T1BDRH = (uint8_t)(Motor_pwnData >> 8);   // duty High
        T1BDRL = (uint8_t)(Motor_pwnData);  	    // duty Low

        Motor_lastSpeed = Motor_CurSpeed;
    }
}

