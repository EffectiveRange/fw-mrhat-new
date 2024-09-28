/*
 * File:   timers.h
 * Author: emtszeb
 *
 * Created on August 2, 2024, 5:41 PM
 */

#ifndef TIMERS_H
#define TIMERS_H
#include "system.h"
#include "tasks.h"
#ifdef __cplusplus
extern "C" {
#endif
volatile uint64_t timer_blink_period;

uint64_t GetTimeMs(void);
void DelayMS(uint32_t delay_ms);
void MiliSecTimerOverflow(void);

typedef struct {
    TaskId task_id;
    task_run_fun task_cb;
    uint64_t period;
    uint64_t act_value;
    bool enabled;
} timer_callbact_t;

typedef enum { TIMER_CB_PI_MONITOR, NUM_TIMER_CB } timer_cb_index_t;
timer_callbact_t timer_callbacks[NUM_TIMER_CB];

void RegisterTimerCallback(task_run_fun task_cb, TaskId taskid,uint64_t period,
                           timer_cb_index_t index) ;
void EnableTimerCallback(timer_cb_index_t index);
void DisableTimerCallback(timer_cb_index_t index);
void SetTimerCallbackPeriod(uint64_t period, timer_cb_index_t index);

#ifdef __cplusplus
}
#endif

#endif /* TIMERS_H */
