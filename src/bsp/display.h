#ifndef __DISPLAY_H__
#define __DISPLAY_H__


#define PORT_FOR_LED_DRIVE()    {P3IO |= (BIT7 | BIT6 | BIT5 | BIT0);P2IO |= (BIT6 | BIT7);}

typedef union
{
    uint8_t leds;
    struct
    {
        uint8_t uvEn:1;
        uint8_t aionEn:1;
        uint8_t preEn:1;
        uint8_t spEn:1;
        uint8_t powerBlueEn:1;
        uint8_t powerOrangeEn:1;
        uint8_t timeOrangeEn:1;
        uint8_t timeBlueEn:1;
    }led;
}LedEn_t;


#define DISPLAY_DATA_LEN    0x3f

#define A_ICON_SERVICE      Display_Datas[0x0d] |= 0x0f
#define A_ICON_AION         Display_Datas[0x0C] |= 0xF0
#define A_ICON_WIFI         Display_Datas[0x2C] |= BIT5;\
                            Display_Datas[0x35] |= (BIT3 | BIT4 | BIT5)
#define A_ICON_UV           Display_Datas[0x08] |= BIT0;\
                            Display_Datas[0x0D] |= (BIT4 | BIT5 | BIT6)
#define A_ICON_CLEAN        Display_Datas[0x30] |= (BIT0 | BIT1);\
                            Display_Datas[0x35] |= (BIT2 | BIT6)

#define A_CIRCLE_MID        Display_Datas[0x25] |= BIT0

#define A_CIRCLE_VANE1      Display_Datas[0x21] |= (BIT1 | BIT2 | BIT3)

#define A_CIRCLE_VANE2      Display_Datas[0x20] |= (BIT6 | BIT7);\
                            Display_Datas[0x21] |= BIT0
#define A_CIRCLE_VANE3      Display_Datas[0x20] |= (BIT3 | BIT4 | BIT5)
#define A_CIRCLE_VANE4      Display_Datas[0x20] |= (BIT0 | BIT1 | BIT2)
#define A_CIRCLE_VANE5      Display_Datas[0x25] |= (BIT4 | BIT5 | BIT6)
#define A_CIRCLE_VANE6      Display_Datas[0x25] |= (BIT1 | BIT2 | BIT3)


#define A_LABEL_PM25        Display_Datas[0x14] |= BIT3;\
                            Display_Datas[0x18] |= BIT6;\
                            Display_Datas[0x1D] |= BIT1


#define A_LABEL_TIMER_ON    Display_Datas[0x38]  |= BIT1;\
                            Display_Datas[0x39]  |= BIT5

#define A_LABEL_T_ON_1      Display_Datas[0x38]  |= BIT0
#define A_LABEL_T_ON_2      Display_Datas[0x3d]  |= BIT6
#define A_LABEL_T_ON_3      Display_Datas[0x3d]  |= BIT5

#define A_LABEL_TIMER_OFF   Display_Datas[0x3d]  |= (BIT3 | BIT4)

#define A_LABEL_T_OFF_1     Display_Datas[0x3d]  |= BIT2
#define A_LABEL_T_OFF_2     Display_Datas[0x3d]  |= BIT1
#define A_LABEL_T_OFF_3     Display_Datas[0x3d]  |= BIT0

#define A_LABEL_H           Display_Datas[0x28] |= BIT6;\
                            Display_Datas[0x2d] |= BIT5
#define A_LABEL_T           Display_Datas[0x29] |= BIT6

#define A_LABEL_FAN_SP      Display_Datas[0x21] |= BIT4;\
                            Display_Datas[0x24] |= BIT7
#define A_LABEL_AUTO        Display_Datas[0x24] |= (BIT0 | BIT1);\
                            Display_Datas[0x10] |= BIT0;


#define A_LABEL_COL         Display_Datas[0x3C] |= (BIT7 | BIT6)




typedef enum
{
    LED_OFF,
    LED_ON = 0xff
}LedSta_t;

typedef enum
{
    CIRCLE_SP_1,
    CIRCLE_SP_2,
    CIRCLE_SP_3,
    CIRCLE_SP_4,
    CIRCLE_SP_5,
    CIRCLE_SP_6,
}DispCircleSp_t;



extern uint8_t Display_Datas[DISPLAY_DATA_LEN];
extern bool_t Display_FlashOn;
extern bool_t Display_IsLowLight;
extern LedEn_t Display_LedsEn;

void Display_SetAll(LedSta_t sta);
void Display_SetLine(void);
void Display_CircleRun(DispCircleSp_t sp);
void Display_DustLevel(uint8_t level);
void Display_SpLevel(SpOptions_t level);

void Display_ShowTemperature(uint8_t t);
void Display_ShowHumidity(uint8_t h);
void Display_DustData(uint16_t pm);
void Display_ShowRT(uint8_t h,uint8_t m,bool_t hFlashEn,bool_t mFlashEn);

void Display_ShowNumInRTCArea(uint16_t num);

void Display_LedDrive(bool_t);

void Display_Ctr(void);

#endif


