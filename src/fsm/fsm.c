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
    /* 状态退出事件处理 */
    if (currentStateExitEventEnable)
    {
        (currentState->exitEventHandler)();
    }

    /* 状态切换 */
    currentState = state;

    /* 状态进入事件处理 */
    if (desStateEnterEventEnable)
    {
        (currentState->enterEventHandler)();
    }
}




