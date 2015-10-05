#include "heads.h"

void main(void)
{
    static idata uint8_t taskIndex;
    static idata uint8_t taskIndexExt;

    static idata uint8_t HSampleT_Cnt;

    BSP_Init();
    Buzz_DeviceInit();
    Uart_MemoryInit();
    Protocol_Init();
    Humidity_DeviceInit();

    Aip1944_BrightnessLevel = BRIGHTNESS_LEVEL5;

    State_TransitionTo(&State_Test, TRUE, FALSE);

    Buzz_Set(1, 10, 15);

    while (TRUE)
    {
        WDTCR |= 0x20;

        Uart_RxDatsDeal();

        if (HSampleT_Cnt > 25)
        {
            if (SquGen_Cnt >= 6)
            {
                Humidity_AdHBuf = Utils_GetAdValue(2);
                HSampleT_Cnt = 0;
            }
        }

        if (SysTick_4ms)
        {
            SysTick_4ms = FALSE;

            if (IR_Changed)
            {
                IR_Changed = FALSE;

                currentState->irData_handler();
            }

            taskIndex++;
            taskIndexExt++;

            HSampleT_Cnt++;

            switch (taskIndex)
            {
            case 1:

            	break;
            case 2:
                Buzz_Ctr();
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

                Humidity_GetCurrentT();
                Humidity_GetCurrentH();
            }
        }
    }
}