#ifndef MOTOR_H
#define MOTOR_H

#define SP(n)   ((n << 2) / 120)

extern volatile uint16_t Motor_CurSpeed;
extern uint16_t Motor_ExpectSpeed;
extern volatile bool_t Motor_IsRunning;


void Motor_Stop(void);
void Motor_Run(uint16_t rpm);
void Motor_Ctr(void);

#endif
