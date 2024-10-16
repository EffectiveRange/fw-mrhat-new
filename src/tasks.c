#include <stdlib.h>
#include "system.h"
#include "tasks.h"

static volatile  TaskDescr _tasks[TASK_NUM] = {};
static volatile int _has_task = 0;

void TASKS_Initialize() {
    for (int i = 0; i < TASK_NUM; ++i) {
        _tasks[i].task_fn = NULL;
        _tasks[i].task_state = NULL;
        _tasks[i].suspended = 0;

    }
    _has_task = 0;
}

void add_task( TaskId id, task_run_fun fn, void* state) {
    const int diff = _tasks[id].task_fn == NULL || _tasks[id].suspended ? 1 : 0;
    _tasks[id].task_fn = fn;
    _tasks[id].task_state = state;
    _tasks[id].suspended = 0;
    _has_task += diff;
}

void resume_task( TaskId id) {
    add_task(id, _tasks[id].task_fn, _tasks[id].task_state);
}

void rm_task( TaskId id) {
    const int diff = _tasks[id].task_fn != NULL && _tasks[id].suspended == 0 ? 1 : 0;
    _tasks[id].task_fn = NULL;
    _tasks[id].task_state = NULL;
    _tasks[id].suspended = 0;
    _has_task -= diff;
}

void suspend_task( TaskId id) {
    if (_tasks[id].task_fn != NULL) {
        _tasks[id].suspended = 1;
        _has_task -= 1;
    }
}

static void __idle() {
    CPUDOZEbits.IDLEN = 1;
    SLEEP();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
}

void run_tasks() {
    while (1) {
        if (_has_task > 0) {
            for (int i = 0; i < TASK_NUM; ++i) {
                if (_tasks[i].task_fn != NULL && _tasks[i].suspended == 0) {
                    _tasks[i].task_fn(&_tasks[i]);
                }
            }
        } else {
            __idle();
        }
    }
}

