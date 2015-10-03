#ifndef __KEY_H__
#define __KEY_H__

#define KEY_NULL    0

#define KEY_POWER   BIT5    // ���ؼ���ֵ
#define KEY_SP      BIT3    // ���ټ���ֵ
#define KEY_UV      BIT1    // UV����
#define KEY_AION    BIT4    // �����Ӱ���
#define KEY_BOOK    BIT0    // ԤԼ����ֵ
#define KEY_CLOCK   BIT2    // ��ʱ����ֵ

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


