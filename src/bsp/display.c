#include "heads.h"


uint8_t Display_Datas[DISPLAY_DATA_LEN];

bool_t Display_FlashOn;
bool_t Display_IsLowLight;
LedEn_t Display_LedsEn;


void Display_SetAll(LedSta_t sta)
{
    uint8_t i;

    for (i = 0; i <= DISPLAY_DATA_LEN;i++)
    {
        Display_Datas[i] = sta;
    }
}


void Display_SetLine(void)
{
    Display_Datas[0] = 0xff;
    Display_Datas[1] = 0xff;
    Display_Datas[2] = 0xff;

    Display_Datas[4] = 0xff;
    Display_Datas[5] = 0xff;
    Display_Datas[6] = 0xff;

    Display_Datas[8] |= 0xfe;
    Display_Datas[9] = 0xff;
    Display_Datas[10] = 0xff;
}

void Display_CircleRun(DispCircleSp_t sp)
{
    static uint8_t cnt;
    static bool_t disIndex;
    cnt ++;
    if (cnt >= sp)
    {
        cnt = 0;

        disIndex = !disIndex;
    }

    A_CIRCLE_MID;

    if (disIndex)
    {
        A_CIRCLE_VANE1;
        A_CIRCLE_VANE3;
        A_CIRCLE_VANE5;
    }
    else
    {
        A_CIRCLE_VANE2;
        A_CIRCLE_VANE4;
        A_CIRCLE_VANE6;
    }
/*
    switch(disIndex)
    {
    case 6:
        A_CIRCLE_VANE6;
    case 5:
        A_CIRCLE_VANE5;
    case 4:
        A_CIRCLE_VANE4;
    case 3:
        A_CIRCLE_VANE3;
    case 2:
        A_CIRCLE_VANE2;
    case 1:
    case 0:
        A_CIRCLE_VANE1;
        break;
    case 7:
    case 8:
        break;
    default:
        disIndex = 0;
        break;
    }
    */
}

void Display_DustLevel(uint8_t level)
{
    switch (level)
    {
    case 6:
        Display_Datas[0x10] |= (BIT1 | BIT2);
    case 5:
        Display_Datas[0x10] |=BIT3;
    case 4:
        Display_Datas[0x10] |=BIT4;
    case 3:
        Display_Datas[0x10] |=BIT5;
    case 2:
        Display_Datas[0x10] |=BIT6;
    case 1:
        Display_Datas[0x10] |=BIT7;
        break;
    default:
        break;
    }
}

void Display_SpLevel(SpOptions_t level)
{
    switch (level)
    {
    case SP_HIGH:
        Display_Datas[0x24] |= BIT2;
    case SP_MID:
        Display_Datas[0x24] |= BIT3;
    case SP_LOW:
        Display_Datas[0x24] |= BIT4;
    case SP_QUIET:
        Display_Datas[0x24] |= BIT5;
    case SP_SLEEP:
        Display_Datas[0x24] |= BIT6;
        break;
    default:
        break;
    }
}


#define T_COM10 Display_Datas[0x2d]     /* SEG_a - SEG_e */
#define T_COM10_EXT Display_Datas[0x2C] /* SEG_f - SEG_g */
#define T_COM1  Display_Datas[0x2c]     /* SEG_a - SEG_e */
#define T_COM1_EXT  Display_Datas[0x31] /* SEG_f - SEG_g */

#define T_COM10_SEG_A   BIT4
#define T_COM10_SEG_B   BIT3
#define T_COM10_SEG_C   BIT2
#define T_COM10_SEG_D   BIT1
#define T_COM10_SEG_E   BIT0

#define T_COM10_EXT_SEG_F   BIT7
#define T_COM10_EXT_SEG_G   BIT6

#define T_COM1_SEG_A    BIT4
#define T_COM1_SEG_B    BIT3
#define T_COM1_SEG_C    BIT2
#define T_COM1_SEG_D    BIT1
#define T_COM1_SEG_E    BIT0

#define T_COM1_EXT_SEG_F    BIT6
#define T_COM1_EXT_SEG_G    BIT5

code uint8_t T_COM10_nums[] =
{
    (T_COM10_SEG_A | T_COM10_SEG_B | T_COM10_SEG_C | T_COM10_SEG_D | T_COM10_SEG_E),
    (T_COM10_SEG_B | T_COM10_SEG_C),
    (T_COM10_SEG_A | T_COM10_SEG_B | T_COM10_SEG_D | T_COM10_SEG_E),
    (T_COM10_SEG_A | T_COM10_SEG_B | T_COM10_SEG_C | T_COM10_SEG_D),
    (T_COM10_SEG_B | T_COM10_SEG_C),
    (T_COM10_SEG_A | T_COM10_SEG_C | T_COM10_SEG_D),
    (T_COM10_SEG_A | T_COM10_SEG_C | T_COM10_SEG_D | T_COM10_SEG_E),
    (T_COM10_SEG_A | T_COM10_SEG_B | T_COM10_SEG_C),
    (T_COM10_SEG_A | T_COM10_SEG_B | T_COM10_SEG_C | T_COM10_SEG_D | T_COM10_SEG_E),
    (T_COM10_SEG_A | T_COM10_SEG_B | T_COM10_SEG_C | T_COM10_SEG_D)
};

code uint8_t T_COM10_EXT_nums[] =
{
    (T_COM10_EXT_SEG_F),
    (0),
    (T_COM10_EXT_SEG_G),
    (T_COM10_EXT_SEG_G),
    (T_COM10_EXT_SEG_F | T_COM10_EXT_SEG_G),
    (T_COM10_EXT_SEG_F | T_COM10_EXT_SEG_G),
    (T_COM10_EXT_SEG_F | T_COM10_EXT_SEG_G),
    (0),
    (T_COM10_EXT_SEG_F | T_COM10_EXT_SEG_G),
    (T_COM10_EXT_SEG_F | T_COM10_EXT_SEG_G),
};

code uint8_t T_COM1_nums[] =
{
    (T_COM1_SEG_A | T_COM1_SEG_B | T_COM1_SEG_C | T_COM1_SEG_D | T_COM1_SEG_E),
    (T_COM1_SEG_B | T_COM1_SEG_C),
    (T_COM1_SEG_A | T_COM1_SEG_B | T_COM1_SEG_D | T_COM1_SEG_E),
    (T_COM1_SEG_A | T_COM1_SEG_B | T_COM1_SEG_C | T_COM1_SEG_D),
    (T_COM1_SEG_B | T_COM1_SEG_C),
    (T_COM1_SEG_A | T_COM1_SEG_C | T_COM1_SEG_D),
    (T_COM1_SEG_A | T_COM1_SEG_C | T_COM1_SEG_D | T_COM1_SEG_E),
    (T_COM1_SEG_A | T_COM1_SEG_B | T_COM1_SEG_C),
    (T_COM1_SEG_A | T_COM1_SEG_B | T_COM1_SEG_C | T_COM1_SEG_D | T_COM1_SEG_E),
    (T_COM1_SEG_A | T_COM1_SEG_B | T_COM1_SEG_C | T_COM1_SEG_D),
};

code uint8_t T_COM1_EXT_nums[] =
{
    (T_COM1_EXT_SEG_F),
    (0),
    (T_COM1_EXT_SEG_G),
    (T_COM1_EXT_SEG_G),
    (T_COM1_EXT_SEG_F | T_COM1_EXT_SEG_G),
    (T_COM1_EXT_SEG_F | T_COM1_EXT_SEG_G),
    (T_COM1_EXT_SEG_F | T_COM1_EXT_SEG_G),
    (0),
    (T_COM1_EXT_SEG_F | T_COM1_EXT_SEG_G),
    (T_COM1_EXT_SEG_F | T_COM1_EXT_SEG_G)
};

/*
#define NUM_0       (SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F)
#define NUM_1       (SEG_B | SEG_C)
#define NUM_2       (SEG_A | SEG_B | SEG_D | SEG_E | SEG_G)
#define NUM_3       (SEG_A | SEG_B | SEG_C | SEG_D | SEG_G)
#define NUM_4       (SEG_B | SEG_C | SEG_F | SEG_G)
#define NUM_5       (SEG_A | SEG_C | SEG_D | SEG_F | SEG_G)
#define NUM_6       (SEG_A | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G)
#define NUM_7       (SEG_A | SEG_B | SEG_C)
#define NUM_8       (SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G)
#define NUM_9       (SEG_A | SEG_B | SEG_C | SEG_D | SEG_F | SEG_G)
*/

void Display_ShowTemperature(uint8_t t)
{
    uint8_t v10;
    uint8_t v1;

    if (t > 99)
    {
        t = 99;
    }

    v10 = t / 10;
    v1 = t % 10;

    if (v10 != 0)
    {
        T_COM10 |= T_COM10_nums[v10];
        T_COM10_EXT |= T_COM10_EXT_nums[v10];
    }
    T_COM1 |= T_COM1_nums[v1];
    T_COM1_EXT |= T_COM1_EXT_nums[v1];
}


#define H_COM10 Display_Datas[0x29]     /* SEG_a - SEG_f */
#define H_COM10_EXT Display_Datas[0x28] /* SEG_g */
#define H_COM1  Display_Datas[0x28]     /* SEG_a - SEG_f */
#define H_COM1_EXT  Display_Datas[0x2D] /* SEG_g */


#define H_COM10_SEG_A   BIT5
#define H_COM10_SEG_B   BIT4
#define H_COM10_SEG_C   BIT3
#define H_COM10_SEG_D   BIT2
#define H_COM10_SEG_E   BIT1
#define H_COM10_SEG_F   BIT0

#define H_COM10_EXT_SEG_G   BIT7

#define H_COM1_SEG_A    BIT5
#define H_COM1_SEG_B    BIT4
#define H_COM1_SEG_C    BIT3
#define H_COM1_SEG_D    BIT2
#define H_COM1_SEG_E    BIT1
#define H_COM1_SEG_F    BIT0

#define H_COM1_EXT_SEG_G    BIT6

code uint8_t H_COM10_nums[] =
{
    (H_COM10_SEG_A | H_COM10_SEG_B | H_COM10_SEG_C | H_COM10_SEG_D | H_COM10_SEG_E | H_COM10_SEG_F),
    (H_COM10_SEG_B | H_COM10_SEG_C),
    (H_COM10_SEG_A | H_COM10_SEG_B | H_COM10_SEG_D | H_COM10_SEG_E),
    (H_COM10_SEG_A | H_COM10_SEG_B | H_COM10_SEG_C | H_COM10_SEG_D),
    (H_COM10_SEG_B | H_COM10_SEG_C | H_COM10_SEG_F),
    (H_COM10_SEG_A | H_COM10_SEG_C | H_COM10_SEG_D | H_COM10_SEG_F),
    (H_COM10_SEG_A | H_COM10_SEG_C | H_COM10_SEG_D | H_COM10_SEG_E | H_COM10_SEG_F),
    (H_COM10_SEG_A | H_COM10_SEG_B | H_COM10_SEG_C),
    (H_COM10_SEG_A | H_COM10_SEG_B | H_COM10_SEG_C | H_COM10_SEG_D | H_COM10_SEG_E | H_COM10_SEG_F),
    (H_COM10_SEG_A | H_COM10_SEG_B | H_COM10_SEG_C | H_COM10_SEG_D | H_COM10_SEG_F)
};

code uint8_t H_COM10_EXT_nums[] =
{
    (0),
    (0),
    (H_COM10_EXT_SEG_G),
    (H_COM10_EXT_SEG_G),
    (H_COM10_EXT_SEG_G),
    (H_COM10_EXT_SEG_G),
    (H_COM10_EXT_SEG_G),
    (0),
    (H_COM10_EXT_SEG_G),
    (H_COM10_EXT_SEG_G)
};

code uint8_t H_COM1_nums[] =
{
    (H_COM1_SEG_A | H_COM1_SEG_B | H_COM1_SEG_C | H_COM1_SEG_D | H_COM1_SEG_E | H_COM1_SEG_F),
    (H_COM1_SEG_B | H_COM1_SEG_C),
    (H_COM1_SEG_A | H_COM1_SEG_B | H_COM1_SEG_D | H_COM1_SEG_E),
    (H_COM1_SEG_A | H_COM1_SEG_B | H_COM1_SEG_C | H_COM1_SEG_D),
    (H_COM1_SEG_B | H_COM1_SEG_C | H_COM1_SEG_F),
    (H_COM1_SEG_A | H_COM1_SEG_C | H_COM1_SEG_D | H_COM1_SEG_F),
    (H_COM1_SEG_A | H_COM1_SEG_C | H_COM1_SEG_D | H_COM1_SEG_E | H_COM1_SEG_F),
    (H_COM1_SEG_A | H_COM1_SEG_B | H_COM1_SEG_C),
    (H_COM1_SEG_A | H_COM1_SEG_B | H_COM1_SEG_C | H_COM1_SEG_D | H_COM1_SEG_E | H_COM1_SEG_F),
    (H_COM1_SEG_A | H_COM1_SEG_B | H_COM1_SEG_C | H_COM1_SEG_D | H_COM1_SEG_F)
};

code uint8_t H_COM1_EXT_nums[] =
{
    (0),
    (0),
    (H_COM1_EXT_SEG_G),
    (H_COM1_EXT_SEG_G),
    (H_COM1_EXT_SEG_G),
    (H_COM1_EXT_SEG_G),
    (H_COM1_EXT_SEG_G),
    (0),
    (H_COM1_EXT_SEG_G),
    (H_COM1_EXT_SEG_G)
};

void Display_ShowHumidity(uint8_t h)
{
    uint8_t v10;
    uint8_t v1;

    if (h > 99)
    {
        h = 99;
    }

    v10 = h / 10;
    v1 = h % 10;

    if (v10 != 0)
    {
        H_COM10 |= H_COM10_nums[v10];
        H_COM10_EXT |= H_COM10_EXT_nums[v10];
    }
    H_COM1 |= H_COM1_nums[v1];
    H_COM1_EXT |= H_COM1_EXT_nums[v1];
}


#define PM_COM1000      Display_Datas[0x15]
#define PM_COM1000_E    Display_Datas[0x14]

#define PM_COM100       Display_Datas[0x14]
#define PM_COM100_E1    Display_Datas[0x19]
#define PM_COM100_E2    Display_Datas[0x18]

#define PM_COM10        Display_Datas[0x18]
#define PM_COM10_E      Display_Datas[0x1D]

#define PM_COM1         Display_Datas[0x1D]
#define PM_COM1_E1      Display_Datas[0x1C]
#define PM_COM1_E2      Display_Datas[0x21]

#define PM_COM1000_SEG_A    BIT6
#define PM_COM1000_SEG_B    BIT5 | BIT4
#define PM_COM1000_SEG_C    BIT3 | BIT2
#define PM_COM1000_SEG_D    BIT1
#define PM_COM1000_SEG_E    BIT0

#define PM_COM1000_E_SEG_E  BIT7
#define PM_COM1000_E_SEG_F  BIT6 | BIT5
#define PM_COM1000_E_SEG_G  BIT4



#define PM_COM100_SEG_A     BIT2
#define PM_COM100_SEG_B     BIT1 | BIT0

#define PM_COM100_E1_SEG_C  BIT6 | BIT5
#define PM_COM100_E1_SEG_D  BIT4
#define PM_COM100_E1_SEG_E  BIT3 | BIT2
#define PM_COM100_E1_SEG_F  BIT1 | BIT0

#define PM_COM100_E2_SEG_G  BIT7



#define PM_COM10_SEG_A      BIT5
#define PM_COM10_SEG_B      BIT4 | BIT3
#define PM_COM10_SEG_C      BIT2 | BIT1
#define PM_COM10_SEG_D      BIT0

#define PM_COM10_E_SEG_E    BIT6 | BIT5
#define PM_COM10_E_SEG_F    BIT4 | BIT3
#define PM_COM10_E_SEG_G    BIT2


#define PM_COM1_SEG_A       BIT0

#define PM_COM1_E1_SEG_B    BIT7 | BIT6
#define PM_COM1_E1_SEG_C    BIT5 | BIT4
#define PM_COM1_E1_SEG_D    BIT3
#define PM_COM1_E1_SEG_E    BIT2 | BIT1
#define PM_COM1_E1_SEG_F    BIT0

#define PM_COM1_E2_SEG_F    BIT6
#define PM_COM1_E2_SEG_G    BIT5


code uint8_t PM_COM1000_nums[] =
{
    (PM_COM1000_SEG_A | PM_COM1000_SEG_B | PM_COM1000_SEG_C | PM_COM1000_SEG_D | PM_COM1000_SEG_E),
    (PM_COM1000_SEG_B | PM_COM1000_SEG_C),
    (PM_COM1000_SEG_A | PM_COM1000_SEG_B | PM_COM1000_SEG_D | PM_COM1000_SEG_E),
    (PM_COM1000_SEG_A | PM_COM1000_SEG_B | PM_COM1000_SEG_C | PM_COM1000_SEG_D),
    (PM_COM1000_SEG_B | PM_COM1000_SEG_C),
    (PM_COM1000_SEG_A | PM_COM1000_SEG_C | PM_COM1000_SEG_D),
    (PM_COM1000_SEG_A | PM_COM1000_SEG_C | PM_COM1000_SEG_D | PM_COM1000_SEG_E),
    (PM_COM1000_SEG_A | PM_COM1000_SEG_B | PM_COM1000_SEG_C),
    (PM_COM1000_SEG_A | PM_COM1000_SEG_B | PM_COM1000_SEG_C | PM_COM1000_SEG_D | PM_COM1000_SEG_E),
    (PM_COM1000_SEG_A | PM_COM1000_SEG_B | PM_COM1000_SEG_C | PM_COM1000_SEG_D)
};

code uint8_t PM_COM1000_E_nums[] =
{
    (PM_COM1000_E_SEG_E | PM_COM1000_E_SEG_F),
    (0),
    (PM_COM1000_E_SEG_E | PM_COM1000_E_SEG_G),
    (PM_COM1000_E_SEG_G),
    (PM_COM1000_E_SEG_F | PM_COM1000_E_SEG_G),
    (PM_COM1000_E_SEG_F | PM_COM1000_E_SEG_G),
    (PM_COM1000_E_SEG_E | PM_COM1000_E_SEG_F | PM_COM1000_E_SEG_G),
    (0),
    (PM_COM1000_E_SEG_E | PM_COM1000_E_SEG_F | PM_COM1000_E_SEG_G),
    (PM_COM1000_E_SEG_F | PM_COM1000_E_SEG_G)
};


code uint8_t PM_COM100_nums[] =
{
    (PM_COM100_SEG_A | PM_COM100_SEG_B ),
    (PM_COM100_SEG_B),
    (PM_COM100_SEG_A | PM_COM100_SEG_B ),
    (PM_COM100_SEG_A | PM_COM100_SEG_B),
    (PM_COM100_SEG_B),
    (PM_COM100_SEG_A),
    (PM_COM100_SEG_A),
    (PM_COM100_SEG_A | PM_COM100_SEG_B),
    (PM_COM100_SEG_A | PM_COM100_SEG_B),
    (PM_COM100_SEG_A | PM_COM100_SEG_B)
};

code uint8_t PM_COM100_E1_nums[] =
{
    (PM_COM100_E1_SEG_C | PM_COM100_E1_SEG_D | PM_COM100_E1_SEG_E | PM_COM100_E1_SEG_F),
    (PM_COM100_E1_SEG_C),
    (PM_COM100_E1_SEG_D | PM_COM100_E1_SEG_E),
    (PM_COM100_E1_SEG_C | PM_COM100_E1_SEG_D),
    (PM_COM100_E1_SEG_C | PM_COM100_E1_SEG_F),
    (PM_COM100_E1_SEG_C | PM_COM100_E1_SEG_D | PM_COM100_E1_SEG_F),
    (PM_COM100_E1_SEG_C | PM_COM100_E1_SEG_D | PM_COM100_E1_SEG_E | PM_COM100_E1_SEG_F),
    (PM_COM100_E1_SEG_C),
    (PM_COM100_E1_SEG_C | PM_COM100_E1_SEG_D | PM_COM100_E1_SEG_E | PM_COM100_E1_SEG_F),
    (PM_COM100_E1_SEG_C | PM_COM100_E1_SEG_D | PM_COM100_E1_SEG_F)
};

code uint8_t PM_COM100_E2_nums[] =
{
    (0),
    (0),
    (PM_COM100_E2_SEG_G),
    (PM_COM100_E2_SEG_G),
    (PM_COM100_E2_SEG_G),
    (PM_COM100_E2_SEG_G),
    (PM_COM100_E2_SEG_G),
    (0),
    (PM_COM100_E2_SEG_G),
    (PM_COM100_E2_SEG_G)
};

code uint8_t PM_COM10_nums[] =
{
    (PM_COM10_SEG_A | PM_COM10_SEG_B | PM_COM10_SEG_C | PM_COM10_SEG_D),
    (PM_COM10_SEG_B | PM_COM10_SEG_C),
    (PM_COM10_SEG_A | PM_COM10_SEG_B | PM_COM10_SEG_D),
    (PM_COM10_SEG_A | PM_COM10_SEG_B | PM_COM10_SEG_C | PM_COM10_SEG_D),
    (PM_COM10_SEG_B | PM_COM10_SEG_C),
    (PM_COM10_SEG_A | PM_COM10_SEG_C | PM_COM10_SEG_D),
    (PM_COM10_SEG_A | PM_COM10_SEG_C | PM_COM10_SEG_D),
    (PM_COM10_SEG_A | PM_COM10_SEG_B | PM_COM10_SEG_C),
    (PM_COM10_SEG_A | PM_COM10_SEG_B | PM_COM10_SEG_C | PM_COM10_SEG_D),
    (PM_COM10_SEG_A | PM_COM10_SEG_B | PM_COM10_SEG_C | PM_COM10_SEG_D)
};

code uint8_t PM_COM10_E_nums[] =
{
    (PM_COM10_E_SEG_E | PM_COM10_E_SEG_F),
    (0),
    (PM_COM10_E_SEG_E | PM_COM10_E_SEG_G),
    (PM_COM10_E_SEG_G),
    (PM_COM10_E_SEG_F | PM_COM10_E_SEG_G),
    (PM_COM10_E_SEG_F | PM_COM10_E_SEG_G),
    (PM_COM10_E_SEG_E | PM_COM10_E_SEG_F | PM_COM10_E_SEG_G),
    (0),
    (PM_COM10_E_SEG_E | PM_COM10_E_SEG_F | PM_COM10_E_SEG_G),
    (PM_COM10_E_SEG_F | PM_COM10_E_SEG_G)
};

code uint8_t PM_COM1_nums[] =
{
    (PM_COM1_SEG_A),
    (0),
    (PM_COM1_SEG_A),
    (PM_COM1_SEG_A),
    (0),
    (PM_COM1_SEG_A),
    (PM_COM1_SEG_A),
    (PM_COM1_SEG_A),
    (PM_COM1_SEG_A),
    (PM_COM1_SEG_A)
};

code uint8_t PM_COM1_E1_nums[] =
{
    (PM_COM1_E1_SEG_B | PM_COM1_E1_SEG_C | PM_COM1_E1_SEG_D | PM_COM1_E1_SEG_E | PM_COM1_E1_SEG_F),
    (PM_COM1_E1_SEG_B | PM_COM1_E1_SEG_C),
    (PM_COM1_E1_SEG_B | PM_COM1_E1_SEG_D | PM_COM1_E1_SEG_E),
    (PM_COM1_E1_SEG_B | PM_COM1_E1_SEG_C | PM_COM1_E1_SEG_D),
    (PM_COM1_E1_SEG_B | PM_COM1_E1_SEG_C | PM_COM1_E1_SEG_F),
    (PM_COM1_E1_SEG_C | PM_COM1_E1_SEG_D | PM_COM1_E1_SEG_F),
    (PM_COM1_E1_SEG_C | PM_COM1_E1_SEG_D | PM_COM1_E1_SEG_E | PM_COM1_E1_SEG_F),
    (PM_COM1_E1_SEG_B | PM_COM1_E1_SEG_C),
    (PM_COM1_E1_SEG_B | PM_COM1_E1_SEG_C | PM_COM1_E1_SEG_D | PM_COM1_E1_SEG_E | PM_COM1_E1_SEG_F),
    (PM_COM1_E1_SEG_B | PM_COM1_E1_SEG_C | PM_COM1_E1_SEG_D | PM_COM1_E1_SEG_F)
};

code uint8_t PM_COM1_E2_nums[] =
{
    (PM_COM1_E2_SEG_F),
    (0),
    (PM_COM1_E2_SEG_G),
    (PM_COM1_E2_SEG_G),
    (PM_COM1_E2_SEG_F | PM_COM1_E2_SEG_G),
    (PM_COM1_E2_SEG_F | PM_COM1_E2_SEG_G),
    (PM_COM1_E2_SEG_F | PM_COM1_E2_SEG_G),
    (0),
    (PM_COM1_E2_SEG_F | PM_COM1_E2_SEG_G),
    (PM_COM1_E2_SEG_F | PM_COM1_E2_SEG_G),
};


void Display_DustData(uint16_t pm)
{
    uint8_t v1000;
    uint8_t v100;
    uint8_t v10;
    uint8_t v1;

    if (pm > 9999)
    {
        pm = 9999;
    }

    v1000 = pm / 1000;
    v100 = (pm / 100) % 10;
    v10 = (pm / 10) % 10;
    v1 = pm % 10;

    
    PM_COM1000 |= PM_COM1000_nums[v1000];
    PM_COM1000_E |= PM_COM1000_E_nums[v1000];
    
    PM_COM100 |= PM_COM100_nums[v100];
    PM_COM100_E1 |= PM_COM100_E1_nums[v100];
    PM_COM100_E2 |= PM_COM100_E2_nums[v100];
    
    PM_COM10 |= PM_COM10_nums[v10];
    PM_COM10_E |= PM_COM10_E_nums[v10];


    PM_COM1 |= PM_COM1_nums[v1];
    PM_COM1_E1 |= PM_COM1_E1_nums[v1];
    PM_COM1_E2 |= PM_COM1_E2_nums[v1];
}





#define RT_COM_H10      Display_Datas[0x31]
#define RT_COM_H10_E    Display_Datas[0x30]

#define RT_COM_H1       Display_Datas[0x0C]
#define RT_COM_H1_E     Display_Datas[0x11]

#define RT_COM_M10      Display_Datas[0x35]
#define RT_COM_M10_E1   Display_Datas[0x34]
#define RT_COM_M10_E2   Display_Datas[0x39]

#define RT_COM_M1       Display_Datas[0x39]
#define RT_COM_M1_E     Display_Datas[0x38]


#define RT_COM_H10_SEG_A    BIT4
#define RT_COM_H10_SEG_B    BIT3 | BIT2
#define RT_COM_H10_SEG_C    BIT1 | BIT0

#define RT_COM_H10_E_SEG_D  BIT7
#define RT_COM_H10_E_SEG_E  BIT6 | BIT5
#define RT_COM_H10_E_SEG_F  BIT4 | BIT3
#define RT_COM_H10_E_SEG_G  BIT2


#define RT_COM_H1_SEG_A     BIT3
#define RT_COM_H1_SEG_B     BIT2 | BIT1
#define RT_COM_H1_SEG_C     BIT0

#define RT_COM_H1_E_SEG_C   BIT6
#define RT_COM_H1_E_SEG_D   BIT5
#define RT_COM_H1_E_SEG_E   BIT4 |BIT3
#define RT_COM_H1_E_SEG_F   BIT2 |BIT1
#define RT_COM_H1_E_SEG_G   BIT0


#define RT_COM_M10_SEG_A    BIT1
#define RT_COM_M10_SEG_B    BIT0

#define RT_COM_M10_E1_SEG_B BIT7
#define RT_COM_M10_E1_SEG_C BIT6 | BIT5
#define RT_COM_M10_E1_SEG_D BIT4
#define RT_COM_M10_E1_SEG_E BIT3 | BIT2
#define RT_COM_M10_E1_SEG_F BIT1 | BIT0

#define RT_COM_M10_E2_SEG_G BIT6



#define RT_COM_M1_SEG_A     BIT4
#define RT_COM_M1_SEG_B     BIT3 | BIT2
#define RT_COM_M1_SEG_C     BIT1 | BIT0

#define RT_COM_M1_E_SEG_D   BIT7
#define RT_COM_M1_E_SEG_E   BIT6 | BIT5
#define RT_COM_M1_E_SEG_F   BIT4 | BIT3
#define RT_COM_M1_E_SEG_G   BIT2


code uint8_t RT_COM_H10_nums[] =
{
    (RT_COM_H10_SEG_A | RT_COM_H10_SEG_B | RT_COM_H10_SEG_C),
    (RT_COM_H10_SEG_B | RT_COM_H10_SEG_C),
    (RT_COM_H10_SEG_A | RT_COM_H10_SEG_B),
    (RT_COM_H10_SEG_A | RT_COM_H10_SEG_B | RT_COM_H10_SEG_C),
    (RT_COM_H10_SEG_B | RT_COM_H10_SEG_C),
    (RT_COM_H10_SEG_A | RT_COM_H10_SEG_C),
    (RT_COM_H10_SEG_A | RT_COM_H10_SEG_C),
    (RT_COM_H10_SEG_A | RT_COM_H10_SEG_B | RT_COM_H10_SEG_C),
    (RT_COM_H10_SEG_A | RT_COM_H10_SEG_B | RT_COM_H10_SEG_C),
    (RT_COM_H10_SEG_A | RT_COM_H10_SEG_B | RT_COM_H10_SEG_C),
    0
};

code uint8_t RT_COM_H10_E_nums[] =
{
    (RT_COM_H10_E_SEG_D | RT_COM_H10_E_SEG_E | RT_COM_H10_E_SEG_F),
    (0),
    (RT_COM_H10_E_SEG_D | RT_COM_H10_E_SEG_E | RT_COM_H10_E_SEG_G),
    (RT_COM_H10_E_SEG_D | RT_COM_H10_E_SEG_G),
    (RT_COM_H10_E_SEG_F | RT_COM_H10_E_SEG_G),
    (RT_COM_H10_E_SEG_D | RT_COM_H10_E_SEG_F | RT_COM_H10_E_SEG_G),
    (RT_COM_H10_E_SEG_D | RT_COM_H10_E_SEG_E | RT_COM_H10_E_SEG_F | RT_COM_H10_E_SEG_G),
    (0),
    (RT_COM_H10_E_SEG_D | RT_COM_H10_E_SEG_E | RT_COM_H10_E_SEG_F | RT_COM_H10_E_SEG_G),
    (RT_COM_H10_E_SEG_D | RT_COM_H10_E_SEG_F | RT_COM_H10_E_SEG_G),
    RT_COM_H10_E_SEG_G
};

code uint8_t RT_COM_H1_nums[] =
{
    (RT_COM_H1_SEG_A | RT_COM_H1_SEG_B | RT_COM_H1_SEG_C),
    (RT_COM_H1_SEG_B | RT_COM_H1_SEG_C),
    (RT_COM_H1_SEG_A | RT_COM_H1_SEG_B),
    (RT_COM_H1_SEG_A | RT_COM_H1_SEG_B | RT_COM_H1_SEG_C),
    (RT_COM_H1_SEG_B | RT_COM_H1_SEG_C),
    (RT_COM_H1_SEG_A | RT_COM_H1_SEG_C),
    (RT_COM_H1_SEG_A | RT_COM_H1_SEG_C),
    (RT_COM_H1_SEG_A | RT_COM_H1_SEG_B | RT_COM_H1_SEG_C),
    (RT_COM_H1_SEG_A | RT_COM_H1_SEG_B | RT_COM_H1_SEG_C),
    (RT_COM_H1_SEG_A | RT_COM_H1_SEG_B | RT_COM_H1_SEG_C),
    0
};

code uint8_t RT_COM_H1_E_nums[] =
{
    (RT_COM_H1_E_SEG_C | RT_COM_H1_E_SEG_D | RT_COM_H1_E_SEG_E | RT_COM_H1_E_SEG_F),
    (RT_COM_H1_E_SEG_C),
    (RT_COM_H1_E_SEG_D | RT_COM_H1_E_SEG_E | RT_COM_H1_E_SEG_G),
    (RT_COM_H1_E_SEG_C | RT_COM_H1_E_SEG_D | RT_COM_H1_E_SEG_G),
    (RT_COM_H1_E_SEG_C | RT_COM_H1_E_SEG_F | RT_COM_H1_E_SEG_G),
    (RT_COM_H1_E_SEG_C | RT_COM_H1_E_SEG_D | RT_COM_H1_E_SEG_F | RT_COM_H1_E_SEG_G),
    (RT_COM_H1_E_SEG_C | RT_COM_H1_E_SEG_D | RT_COM_H1_E_SEG_E | RT_COM_H1_E_SEG_F | RT_COM_H1_E_SEG_G),
    (RT_COM_H1_E_SEG_C),
    (RT_COM_H1_E_SEG_C | RT_COM_H1_E_SEG_D | RT_COM_H1_E_SEG_E | RT_COM_H1_E_SEG_F | RT_COM_H1_E_SEG_G),
    (RT_COM_H1_E_SEG_C | RT_COM_H1_E_SEG_D | RT_COM_H1_E_SEG_F | RT_COM_H1_E_SEG_G),
    RT_COM_H1_E_SEG_G
};


code uint8_t RT_COM_M10_nums[] =
{
    (RT_COM_M10_SEG_A | RT_COM_M10_SEG_B),
    (RT_COM_M10_SEG_B),
    (RT_COM_M10_SEG_A | RT_COM_M10_SEG_B),
    (RT_COM_M10_SEG_A | RT_COM_M10_SEG_B),
    (RT_COM_M10_SEG_B),
    (RT_COM_M10_SEG_A),
    (RT_COM_M10_SEG_A),
    (RT_COM_M10_SEG_A | RT_COM_M10_SEG_B),
    (RT_COM_M10_SEG_A | RT_COM_M10_SEG_B),
    (RT_COM_M10_SEG_A | RT_COM_M10_SEG_B),
    0
};

code uint8_t RT_COM_M10_E1_nums[] =
{
    (RT_COM_M10_E1_SEG_B | RT_COM_M10_E1_SEG_C | RT_COM_M10_E1_SEG_D | RT_COM_M10_E1_SEG_E | RT_COM_M10_E1_SEG_F),
    (RT_COM_M10_E1_SEG_C),
    (RT_COM_M10_E1_SEG_B | RT_COM_M10_E1_SEG_D | RT_COM_M10_E1_SEG_E),
    (RT_COM_M10_E1_SEG_B | RT_COM_M10_E1_SEG_C | RT_COM_M10_E1_SEG_D),
    (RT_COM_M10_E1_SEG_B | RT_COM_M10_E1_SEG_C | RT_COM_M10_E1_SEG_F),
    (RT_COM_M10_E1_SEG_C | RT_COM_M10_E1_SEG_D | RT_COM_M10_E1_SEG_F),
    (RT_COM_M10_E1_SEG_C | RT_COM_M10_E1_SEG_D | RT_COM_M10_E1_SEG_E | RT_COM_M10_E1_SEG_F),
    (RT_COM_M10_E1_SEG_B | RT_COM_M10_E1_SEG_C),
    (RT_COM_M10_E1_SEG_B | RT_COM_M10_E1_SEG_C | RT_COM_M10_E1_SEG_D | RT_COM_M10_E1_SEG_E | RT_COM_M10_E1_SEG_F),
    (RT_COM_M10_E1_SEG_B | RT_COM_M10_E1_SEG_C | RT_COM_M10_E1_SEG_D | RT_COM_M10_E1_SEG_F),
    0
};

code uint8_t RT_COM_M10_E2_nums[] =
{
    (0),
    (0),
    (RT_COM_M10_E2_SEG_G),
    (RT_COM_M10_E2_SEG_G),
    (RT_COM_M10_E2_SEG_G),
    (RT_COM_M10_E2_SEG_G),
    (RT_COM_M10_E2_SEG_G),
    (0),
    (RT_COM_M10_E2_SEG_G),
    (RT_COM_M10_E2_SEG_G),
    RT_COM_M10_E2_SEG_G
};


code uint8_t RT_COM_M1_nums[] =
{
    (RT_COM_M1_SEG_A | RT_COM_M1_SEG_B | RT_COM_M1_SEG_C),
    (RT_COM_M1_SEG_B | RT_COM_M1_SEG_C),
    (RT_COM_M1_SEG_A | RT_COM_M1_SEG_B),
    (RT_COM_M1_SEG_A | RT_COM_M1_SEG_B | RT_COM_M1_SEG_C),
    (RT_COM_M1_SEG_B | RT_COM_M1_SEG_C),
    (RT_COM_M1_SEG_A | RT_COM_M1_SEG_C),
    (RT_COM_M1_SEG_A | RT_COM_M1_SEG_C),
    (RT_COM_M1_SEG_A | RT_COM_M1_SEG_B | RT_COM_M1_SEG_C),
    (RT_COM_M1_SEG_A | RT_COM_M1_SEG_B | RT_COM_M1_SEG_C),
    (RT_COM_M1_SEG_A | RT_COM_M1_SEG_B | RT_COM_M1_SEG_C),
    0
};

code uint8_t RT_COM_M1_E_nums[] =
{
    (RT_COM_M1_E_SEG_D | RT_COM_M1_E_SEG_E | RT_COM_M1_E_SEG_F),
    (0),
    (RT_COM_M1_E_SEG_D | RT_COM_M1_E_SEG_E | RT_COM_M1_E_SEG_G),
    (RT_COM_M1_E_SEG_D | RT_COM_M1_E_SEG_G),
    (RT_COM_M1_E_SEG_F | RT_COM_M1_E_SEG_G),
    (RT_COM_M1_E_SEG_D | RT_COM_M1_E_SEG_F | RT_COM_M1_E_SEG_G),
    (RT_COM_M1_E_SEG_D | RT_COM_M1_E_SEG_E | RT_COM_M1_E_SEG_F | RT_COM_M1_E_SEG_G),
    (0),
    (RT_COM_M1_E_SEG_D | RT_COM_M1_E_SEG_E | RT_COM_M1_E_SEG_F | RT_COM_M1_E_SEG_G),
    (RT_COM_M1_E_SEG_D | RT_COM_M1_E_SEG_F | RT_COM_M1_E_SEG_G),
    RT_COM_M1_E_SEG_G
};


void Display_ShowRT(uint8_t h,uint8_t m,bool_t hFlashEn,bool_t mFlashEn)
{
    uint8_t h10;
    uint8_t h1;

    uint8_t m10;
    uint8_t m1;

    h10 = h / 10;
    h1 = h % 10;
    m10 = m / 10;
    m1 = m % 10;

    if (h > 24)
    {
       h10 = 10;
       h1 = 10;
    }

    if (m > 60)
    {
        m10 = 10;
        m1 = 10;
    }
    if ((!hFlashEn) || Display_FlashOn)
    {
        RT_COM_H10 |= RT_COM_H10_nums[h10];
        RT_COM_H10_E |= RT_COM_H10_E_nums[h10];

        RT_COM_H1 |= RT_COM_H1_nums[h1];
        RT_COM_H1_E |= RT_COM_H1_E_nums[h1];
    }


    if ((!mFlashEn) || Display_FlashOn)
    {
        RT_COM_M10 |= RT_COM_M10_nums[m10];
        RT_COM_M10_E1 |= RT_COM_M10_E1_nums[m10];
        RT_COM_M10_E2 |= RT_COM_M10_E2_nums[m10];

        RT_COM_M1 |= RT_COM_M1_nums[m1];
        RT_COM_M1_E |= RT_COM_M1_E_nums[m1];
    }
}


void Display_ShowNumInRTCArea(uint16_t num)
{
    uint8_t v1000;
    uint8_t v100;
    uint8_t v10;
    uint8_t v1;

    v1000 = num / 1000;
    v100 = (num / 100) % 10;
    v10 = (num / 10) % 10;
    v1 = num % 10;

    /* ��Ҫ��ʾ�����ִ���9999ʱ����ʾ��----�� */
    if (num > 9999)
    {
        v1000 = 10;
        v100 = 10;
        v10 = 10;
        v1 = 10;
    }

    
    RT_COM_H10 |= RT_COM_H10_nums[v1000];
    RT_COM_H10_E |= RT_COM_H10_E_nums[v1000];

    RT_COM_H1 |= RT_COM_H1_nums[v100];
    RT_COM_H1_E |= RT_COM_H1_E_nums[v100];

    RT_COM_M10 |= RT_COM_M10_nums[v10];
    RT_COM_M10_E1 |= RT_COM_M10_E1_nums[v10];
    RT_COM_M10_E2 |= RT_COM_M10_E2_nums[v10];

    RT_COM_M1 |= RT_COM_M1_nums[v1];
    RT_COM_M1_E |= RT_COM_M1_E_nums[v1];
}


#define LED_COM0_ON()       {P3 |= BIT5;P3 |= BIT0;}
#define LED_COM1_ON()       {P3 |= BIT6;P2 |= BIT7;}
#define LED_COM2_ON()       {P3 |= BIT7;P2 |= BIT6;}

#define LED_ALL_COM_OFF()   {P3 &= (RBIT0 & RBIT5 & RBIT6 & RBIT7);P2 &= (RBIT6 & RBIT7);}

#define P1_LED1_0()         P3 &= (~BIT3)
#define P1_LED1_1()         P3 |= BIT3

#define P1_LED2_0()         P3 &= (~BIT4)
#define P1_LED2_1()         P3 |= BIT4

#define P2_LED1_0()         P3 &= (~BIT2)
#define P2_LED1_1()         P3 |= BIT2

#define P2_LED2_0()         P3 &= (~BIT1)
#define P2_LED2_1()         P3 |= BIT1


void Display_LedDrive(bool_t comChangeEn)
{
    static uint8_t scanCnt;

    if (comChangeEn)
    {
         scanCnt ++;
    }

    PORT_FOR_LED_DRIVE();
    LED_ALL_COM_OFF();
    switch(scanCnt)
    {
    case 1:
        if(Display_LedsEn.led.uvEn)
        {
            P1_LED2_0();
        }
        else
        {
            P1_LED2_1();
        }
        if(Display_LedsEn.led.aionEn)
        {
            P2_LED2_0();
        }
        else
        {
            P2_LED2_1();
        }

        LED_COM0_ON();
        break;
    case 2:
        if(Display_LedsEn.led.preEn)
        {
            P1_LED2_0();
        }
        else
        {
            P1_LED2_1();
        }
        if(Display_LedsEn.led.spEn)
        {
            P2_LED2_0();
        }
        else
        {
            P2_LED2_1();
        }
        LED_COM1_ON();
        break;
    case 3:
    default:
        if(Display_LedsEn.led.powerBlueEn)
        {
            P2_LED2_0();
        }
        else
        {
            P2_LED2_1();
        }

        if(Display_LedsEn.led.powerOrangeEn)
        {
            P2_LED1_0();
        }
        else
        {
            P2_LED1_1();
        }

        if(Display_LedsEn.led.timeOrangeEn)
        {
            P1_LED1_0();
        }
        else
        {
            P1_LED1_1();
        }

        if(Display_LedsEn.led.timeBlueEn)
        {
            P1_LED2_0();
        }
        else
        {
            P1_LED2_1();
        }
        LED_COM2_ON();

        scanCnt = 0;
        break;
    }
}


void Display_Ctr(void)
{
    Display_SetAll(LED_OFF);
    Display_LedsEn.leds = 0;

    currentState->displayControl_handler();

    Aip1944_WriteDispDatas(Display_Datas,DISPLAY_DATA_LEN,0);
}

