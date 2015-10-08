#ifndef HEADS_H
#define HEADS_H

#include "MC96F6432.h"
#include "macrodef.h"
#include "typedef.h"

#include "system.h"

#include "it.h"

#include "bsp/bsp_init.h"
#include "bsp/aip1944.h"
#include "bsp/display.h"
#include "bsp/key.h"
#include "bsp/buzzer.h"
#include "bsp/ir.h"
#include "bsp/humidity.h"
#include "bsp/dust.h"
#include "bsp/motor.h"


#include "protocol/fifo.h"
#include "protocol/uart.h"
#include "protocol/protocol.h"


#include "fsm/fsm.h"
#include "fsm/test/state_test.h"
#include "fsm/startup/state_startup.h"
#include "fsm/standby/state_standby.h"
#include "fsm/clock_setting/state_clockSetting.h"
#include "fsm/echeck/state_echeck.h"
#include "fsm/timer_setting/state_timerSetting.h"
#include "fsm/timer_fn_setting/state_timerFnSetting.h"
#include "fsm/timer_lookup/state_timerLookup.h"
#include "fsm/work/state_work.h"


#endif


