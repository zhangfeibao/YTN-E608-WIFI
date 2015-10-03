#ifndef __KEY_H__
#define __KEY_H__

#define KEY_NULL    0

#define KEY_POWER   BIT5    // 开关键键值
#define KEY_SP      BIT3    // 风速键键值
#define KEY_UV      BIT1    // UV按键
#define KEY_AION    BIT4    // 负离子按键
#define KEY_BOOK    BIT0    // 预约键键值
#define KEY_CLOCK   BIT2    // 定时键键值

typedef enum
{
    KEY_ACTION_UNDEFINE,
    KEY_ACTION_SHORT,
    KEY_ACTION_RELEASING,
    KEY_ACTION_LONG,
    KEY_ACTION_INTERVAL
}KeyAction_t;

extern uint8_t Key_Code;
extern KeyAction_t Key_Action;

extern bool_t Key_Released;

void Key_ActionDetect(void);

#endif


