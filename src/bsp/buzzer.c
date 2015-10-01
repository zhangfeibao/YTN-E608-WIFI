#include "heads.h"

static bool_t Buzz_En;

static uint8_t Buzz_times;
static uint8_t Buzz_on;
static uint8_t Buzz_off;

static uint8_t Buzz_cnt;

void Buzz_DeviceInit(void)
{
    Buzz_En = 0;

    Buzz_times = 0;
    Buzz_on = 0;
    Buzz_off = 0;

    Buzz_cnt = 0;
}

void Buzz_Set(uint8_t times, uint8_t on, uint8_t off)
{
    if (off <= on)
    {
        off = on + 1;
    }

    Buzz_times = times;
    Buzz_on = on;
    Buzz_off = off;

    Buzz_cnt = 0;
}

void Buzz_Ctr(void)
{
    if (Buzz_times)
    {
        Buzz_cnt++;

        if (Buzz_cnt < Buzz_on)
        {
            if (!Buzz_En)
            {
                BUZCR |= 0x01;
            }

            Buzz_En = 1;
        }
        else if (Buzz_cnt < Buzz_off)
        {
            if (Buzz_En)
            {
                BUZCR &= (~0x01);
            }
            Buzz_En = 0;
        }
        else
        {
            Buzz_times--;
            Buzz_cnt = 0;
        }
    }
    else
    {
        Buzz_En = 0;
    }
}