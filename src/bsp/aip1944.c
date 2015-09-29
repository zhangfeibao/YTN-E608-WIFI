#include "heads.h"

AIP1944_Brightness_t Aip1944_BrightnessLevel;


/* �˿ڲ����궨�� */
#define STB_SET P40 = 1
#define STB_CLR P40 = 0
#define CLK_SET P41 = 1
#define CLK_CLR P41 = 0
#define DIO_SET P42 = 1
#define DIO_CLR P42 = 0

/* ��ʾģʽ���� */
enum
{
    SEG8_GRID24 = 0x00,
    SEG9_GRID23 = 0x01,
    SEG10_GRID22 = 0x02,
    SEG11_GRID21 = 0x03,
    SEG12_GRID20 = 0x04,
    SEG13_GRID19 = 0x05,
    SEG14_GRID18 = 0x06,
    SEG15_GRID17 = 0x07,
    SEG16_GRID16 = 0x08,
};

/* �������ã���������д��ʽ */
enum
{
    W_AUTO_INC = 0x40,  /* ��ַ�Զ���1 */
    W_FIX = 0x44,       /* �̶���ַ */
};


static void AIP1944_WriteByte(uint8_t _byte)
{
    uint8_t i;

    for (i = 0; i < 8;i++)
    {
        if (_byte & 0x01)
        {
            DIO_SET;
        }
        else
        {
            DIO_CLR;
        }

        CLK_SET;

        _byte >>= 1;

        CLK_CLR;
    }
}

void Aip1944_WriteDispDatas(uint8_t* dats, uint8_t len, uint8_t addr)
{
    uint8_t i;

    STB_CLR;
    AIP1944_WriteByte(SEG16_GRID16);
    STB_SET;
    STB_CLR;
    AIP1944_WriteByte(addr | 0xC0);
    for (i = 0; i < len; i++)
    {
        AIP1944_WriteByte(dats[i]);
    }
    STB_SET;

    STB_CLR;
    AIP1944_WriteByte(Aip1944_BrightnessLevel);
    STB_SET;
}

