/* 
 * File:   tasks.h
 * Author: fecja
 *
 * Created on November 25, 2023, 9:35 AM
 */

#ifndef TASKS_H
#define	TASKS_H
#include "system.h"
#ifdef	__cplusplus
extern "C" {
#endif

    typedef  enum{
        TASK_ONOFF = 0,
        TASK_I2C_WAKEUP,
        TASK_I2C_SWITCH_MODE,
        TASK_DEMO,
        TASK_SHUTDOWN_BUTTON_PRESSED,
        TASK_PI_SHUTDOWN_MONITOR,
        TASK_POWER_IC_SYSTEM_RESET,
        TASK_PI_SHUTDOWN_OR_WAKEUP,
        TASK_PI_MONITOR,
        TASK_SET_MCU_INT_PIN,
        TASK_WAKE_UP_PI,
        TASK_CHECK_RTC,
        TASK_CHECK_BQ_IRQ,
        TASK_CHECK_BAT_AVAIL,
        TASK_START_MES_IBAT,
        TASK_NUM
    }TaskId;

    typedef struct TaskDescr{
        void (*task_fn)(volatile  struct TaskDescr*);
        void* task_state;
        uint8_t suspended;
    }TaskDescr;
    typedef void (*task_run_fun)(volatile  TaskDescr*);

    void TASKS_Initialize(void);

    // Add task to main thread
    // the task descriptor will be passed to the task,
    // so it can freely manipulate the opaque state
    // and modify the continuation function for the next callback
    // The callback must not set or unset the callback pointer!
    // The only valid operation is to set to a different valid callback
    // The state pointer can be manipulated freely
    void add_task( TaskId id, task_run_fun fn, void* state);

    // Resume a suspended task
    void resume_task( TaskId id);

    // remove a task from scheduling
    void rm_task( TaskId id);

    // Suspend a task, this keeps the last valid state and callback
    // pointer intact
    void suspend_task( TaskId id);

    // entry point from main thread
    void run_tasks(void);



#ifdef	__cplusplus
}
#endif

#endif	/* TASKS_H */

