#include "heads.h"

void main(void)
{
    static idata uint8_t taskIndex;
    static idata uint8_t taskIndexExt;

    BSP_Init();

    Aip1944_BrightnessLevel = BRIGHTNESS_LEVEL5;

    while (TRUE)
    {
        WDTCR |= 0x20;

        if (SysTick_4ms)
        {

            taskIndex++;
            taskIndexExt++;

            switch (taskIndex)
            {
            case 1:

            	break;
            case 2:

                break;
            case 3:

                break;
            case 4:

                break;
            case 5:
            default:
                taskIndex = 0;

                break;
            }

            taskIndexExt++;
            if (taskIndexExt > 15)
            {
                taskIndexExt = 0;

                Display_Ctr();
            }
        }
    }
}