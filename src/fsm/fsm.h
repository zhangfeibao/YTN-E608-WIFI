#ifndef __FSM_H__
#define __FSM_H__

#include "typedef.h"

typedef struct
{
    task_t keyAction_handler;
    task_t displayControl_handler;
    task_t taskExe_handler;
    task_t timeTick_handler;
    task_t motorCtr_handler;
    task_t irData_handler;
    task_t rxData_handler;

    task_t enterEventHandler;
    task_t exitEventHandler;
}State_t;

extern State_t code* currentState;

void State_TransitionTo(State_t code *state,
    bool_t desStateEnterEventEnable,
    bool_t currentStateExitEventEnable);


#endif


