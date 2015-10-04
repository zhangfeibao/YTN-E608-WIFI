#include "heads.h"

uint8_t Key_Code;
KeyAction_t Key_Action;

bool_t Key_Released;

#define PORT_FOR_KEY_SCAN()   //{P1CONH &= (~0XFC);P4CONH &= (~0X3F);}
static uint8_t Key_Read(void)
{
    uint8_t keyDat;

    /* ¹Ø±ÕLED */
    P4 |= (BIT0 | BIT1 | BIT3);
    P2 |= BIT7;

    NOP(); NOP(); NOP(); NOP();
    PORT_FOR_KEY_SCAN();
    NOP(); NOP(); NOP(); NOP();

    keyDat = KEY_NULL;

    if ((P1 & BIT7) == 0)
    {
        keyDat |= BIT0;
    }

    if ((P1 & BIT6) == 0)
    {
        keyDat |= BIT1;
    }

    if ((P1 & BIT5) == 0)
    {
        keyDat |= BIT2;
    }

    if ((P4 & BIT4) == 0)
    {
        keyDat |= BIT3;
    }

    if ((P4 & BIT5) == 0)
    {
        keyDat |= BIT4;
    }

    if ((P4 & BIT6) == 0)
    {
        keyDat |= BIT5;
    }

    PORT_FOR_LED_DRIVE();

    return keyDat;
}

void Key_ActionDetect(void)
{
    uint8_t Key_codeBuf;

    static uint8_t Key_stateHoldCnt;
    static uint8_t Key_stateHoldCntExt;

    Key_codeBuf = Key_Read();
    if (Key_Code != Key_codeBuf)
    {
        if (Key_codeBuf == KEY_NULL && Key_stateHoldCnt > 5 && Key_stateHoldCnt < 150)
        {
            Key_Action = KEY_ACTION_RELEASING;
            currentState->keyAction_handler();
        }
        Key_Code = Key_codeBuf;
    }
    else
    {
        if (Key_Code == 0)
        {
            Key_stateHoldCnt = 0;
            Key_stateHoldCntExt = 0;

            Key_Released = TRUE;
        }
        else
        {
            Key_Action = KEY_ACTION_UNDEFINE;
            Key_stateHoldCnt++;
            Key_stateHoldCntExt++;

            if (Key_stateHoldCntExt > 60)
            {
                Key_stateHoldCntExt = 45;
                Key_Action = KEY_ACTION_INTERVAL;
                currentState->keyAction_handler();
            }

            if (Key_stateHoldCnt == 5)
            {
                Key_Action = KEY_ACTION_SHORT;
            }
            else if (Key_stateHoldCnt == 248)
            {
                Key_Action = KEY_ACTION_LONG;
            }
            else
            {
                if (Key_stateHoldCnt > 250)
                {
                    Key_stateHoldCnt = 250;
                }
                return;
            }

            currentState->keyAction_handler();
        }
    }
}

