#include "heads.h"

volatile fastBool_t SysTick_4ms;

void INT_USI1_Rx() interrupt 3
{
    // USI1 Rx interrupt
    // TODO: add your code here
}

void INT_USI1_Tx() interrupt 4
{
    // USI1 Tx interrupt
    // TODO: add your code here
}

void INT_WT() interrupt 20
{
    // Watch timer interrupt
    // TODO: add your code here
}

void INT_BIT() interrupt 22
{
    // BIT interrupt
    static idata uint8_t t_128usCnt;

    t_128usCnt++;
    if (t_128usCnt >= 50)
    {
        t_128usCnt = 0;

        SysTick_4ms = TRUE;
    }
}