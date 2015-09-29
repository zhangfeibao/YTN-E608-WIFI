#ifndef AIP1944_H
#define AIP1944_H

/** 显示寄存器地址和显示模式说明
  * ----------------------------------------------------------------------------
  * MCU通过串行接口将显示数据写入到显示寄存器,地址从00H - 2FH共48个显示字节单元,分别与
  * 芯片SEG和GRID管脚所接的LED对应，对应关系如下所示：
  * 写LED显示数据时，按照显示地址从低位到高位，从数据字节的低位到高位操作。
  * SEG1|SEG2|SEG3|SEG4|SEG5|SEG6|SEG7|SEG8|SEG9|SEG10|SEG11|SEG12|SEG13|SEG14|SEG15|SEG16|SEG17|SEG18|SEG19|SEG20|SEG21|SEG22|SEG23|SEG24 | 无效数据
  * B0  |B1  |B2  |B3  |B4  |B5  |B6  |B7  |B0  |B1   |B2   |B3   |B4   |B5   |B6   |B7   |B0   |B1   |B2   |B3   |B4   |B5   |B6   |B7    | --------
  * -------------------------------------------------------------------------------------------------------------------------------------------------
  *             00H                   |                  01H                        |                       02H                            |   03H   |GRID1
  *             04H                   |                  05H                        |                       06H                            |   07H   |GRID2
  *             08H                   |                  09H                        |                       0AH                            |   0BH   |GRID3
  *             0CH                   |                  0DH                        |                       0EH                            |   0FH   |GRID4
  *             10H                   |                  11H                        |                       12H                            |   13H   |GRID5
  *             14H                   |                  15H                        |                       16H                            |   17H   |GRID6
  *             18H                   |                  19H                        |                       1AH                            |   1BH   |GRID7
  *             1CH                   |                  1DH                        |                       1EH                            |   1FH   |GRID8
  *             20H                   |                  21H                        |                       22H                            |   23H   |GRID9
  *             24H                   |                  25H                        |                       26H                            |   27H   |GRID10
  *             28H                   |                  29H                        |                       2AH                            |   2BH   |GRID11
  *             2CH                   |                  2DH                        |                       2EH                            |   2FH   |GRID12
  *             30H                   |                  31H                        |                       32H                            |   33H   |GRID13
  *             34H                   |                  35H                        |                       36H                            |   37H   |GRID14
  *             38H                   |                  39H                        |                       3AH                            |   3BH   |GRID15
  *             3CH                   |                  3DH                        |                       3EH                            |   3FH   |GRID16
  *
  */

/* 显示控制:level0关显示 level1 - level8 亮度依次增加 */
typedef enum
{
    BRIGHTNESS_LEVEL0 = 0x80,
    BRIGHTNESS_LEVEL1 = 0x88,
    BRIGHTNESS_LEVEL2 = 0x89,
    BRIGHTNESS_LEVEL3 = 0x8A,
    BRIGHTNESS_LEVEL4 = 0x8B,
    BRIGHTNESS_LEVEL5 = 0x8C,
    BRIGHTNESS_LEVEL6 = 0x8D,
    BRIGHTNESS_LEVEL7 = 0x8E,
    BRIGHTNESS_LEVEL8 = 0x8F,
}AIP1944_Brightness_t;

extern AIP1944_Brightness_t Aip1944_BrightnessLevel;
void Aip1944_WriteDispDatas(uint8_t* dats, uint8_t len, uint8_t addr);

#endif


