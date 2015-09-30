#include "heads.h"

static void Task_DoNothing(void) {}
static State_t code doNothing =
{
    Task_DoNothing,
    Task_DoNothing,
    Task_DoNothing,
    Task_DoNothing,
    Task_DoNothing,
    Task_DoNothing,
    Task_DoNothing,

    Task_DoNothing,
    Task_DoNothing
};

State_t code* currentState = (&doNothing);

void State_TransitionTo(State_t code *state,
    bool_t desStateEnterEventEnable,
    bool_t currentStateExitEventEnable)
{
    /* ״̬�˳��¼����� */
    if (currentStateExitEventEnable)
    {
        (currentState->exitEventHandler)();
    }

    /* ״̬�л� */
    currentState = state;

    /* ״̬�����¼����� */
    if (desStateEnterEventEnable)
    {
        (currentState->enterEventHandler)();
    }
}




