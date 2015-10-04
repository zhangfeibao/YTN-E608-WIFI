#ifndef IR_H
#define IR_H

#define REMOTE_RX           P2 & BIT4       // ������������

#define MS13_IR             106         // 125US*104=13MS
#define MS11_IR             90          // 125US*90=11MS
#define MS1_5_IR            15          // 125US*15=1.8MS
#define MS150_IR            1229        // 125US*1229=150MS

#define IR_ON_OFF           0X00FF
#define IR_SPEED_AUTO       0X807F
#define IR_SPEED_GEAR_1     0X40BF
#define IR_SPEED_GEAR_2     0XA05F
#define IR_SPEED_GEAR_3     0X609F
#define IR_SPEED_GEAR_4     0XC03F
#define IR_SPEED_GEAR_5     0X20DF
#define IR_ION              0XE01F
#define IR_UV               0X10EF
#define IR_HV_NONE          0X906F
#define IR_CLEAR            0X50AF
#define IR_WIFI             0XD02F
#define IR_BOOK_CHAGE       0X30CF
#define IR_BOOK_1           0xB04F
#define IR_BOOK_2           0X708F
#define IR_BOOK_3           0XF00F



extern  uint16_t IR_Code;
extern volatile bool_t IR_Changed;

#endif

