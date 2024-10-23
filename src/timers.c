#include <stdint.h>

#include "system.h"
#include "tasks.h"
#include "timers.h"

volatile uint64_t free_run_timer = 0;

uint64_t GetTimeMs(void) { return free_run_timer; }


timer_callbact_t timer_callbacks[NUM_TIMER_CB];

void RegisterTimerCallback(task_run_fun task_cb, TaskId taskid,uint64_t period,
                           timer_cb_index_t index) {
    timer_callbacks[index].task_cb = task_cb;
    timer_callbacks[index].task_id = taskid;
    timer_callbacks[index].period = period;
    timer_callbacks[index].act_value = 0;
    timer_callbacks[index].enabled = false;
}
void EnableTimerCallback(timer_cb_index_t index) {
    timer_callbacks[index].enabled = true;
    timer_callbacks[index].act_value = 0;
}
void DisableTimerCallback(timer_cb_index_t index) {
    timer_callbacks[index].enabled = false;
}
void SetTimerCallbackPeriod(uint64_t period, timer_cb_index_t index) {
    timer_callbacks[index].period = period;
    timer_callbacks[index].act_value = 0;
}

void MiliSecTimerOverflow(void) {
    free_run_timer++;

    for (int i = 0; i < NUM_TIMER_CB; i++) {
        if (timer_callbacks[i].enabled) {
            timer_callbacks[i].act_value++;
            if (timer_callbacks[i].act_value >= timer_callbacks[i].period) {
                timer_callbacks[i].act_value = 0;
//                timer_callbacks[i].task_cb(NULL);
                timer_callbacks[i].enabled = false;
                add_task(timer_callbacks[i].task_id, timer_callbacks[i].task_cb, NULL);
            }
        }
    }
}

void DelayMS(uint32_t delay_ms) {
    uint64_t now_ms = free_run_timer;
    while (free_run_timer < (now_ms + delay_ms));
}