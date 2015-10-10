#include "heads.h"

void initAll(void)
{
    BSP_Init();
    Buzz_DeviceInit();
    Uart_MemoryInit();
    Protocol_Init();
    Humidity_DeviceInit();
    Dust_DeviceInit();

    Aip1944_BrightnessLevel = BRIGHTNESS_LEVEL7;

    State_TransitionTo(&State_Startup, TRUE, FALSE);
    //State_TransitionTo(&State_Test, TRUE, FALSE);

    Buzz_Set(1, 10, 15);

    sei();          	// enable INT.
}

void main(void)
{
    static idata uint8_t taskIndex;
    static idata uint8_t taskIndexExt;

    static idata uint8_t HSampleT_Cnt;

    initAll();

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
                Key_ActionDetect();
                Display_LedDrive(FALSE);
            	break;
            case 2:
                Buzz_Ctr();
                break;
            case 3:
                Dust_DataUpdate();
                break;
            case 4:
                Motor_Ctr();
                break;
            case 5:
                Sys_LoadCtr();
                Sys_EnterStopMode();
            default:
                taskIndex = 0;

                break;
            }

            if (taskIndexExt >= 15)
            {
                taskIndexExt = 0;

                Display_Ctr();

                Humidity_GetCurrentT();
                Humidity_GetCurrentH();
                Dust_LevelUpdate();

                currentState->timeTick_handler();

                if (Time_Flag1s)
                {
                    Time_Flag1s--;
                }

                if (Time_Flag1Min)
                {
                    Time_Flag1Min = FALSE;
                }
            }
        }
    }
}