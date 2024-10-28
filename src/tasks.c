#include <stdlib.h>
#include "pic18f16q20.h"
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
    //When the IDLEN bit is clear, the SLEEP instruction will put the device into full Sleep mode. 
    //When IDLEN is set, the SLEEP instruction will put the device into Idle mode. 
    //In Idle mode, the CPU and memory operations are halted, but the peripheral clocks continue to run. 
    //This mode is similar to Doze mode, except that in Idle both the CPU and program memory are shut off.
    
      CPUDOZEbits.IDLEN = 1;//go to idle
//   CPUDOZEbits.IDLEN = 0; //go to sleep 

    //test doze
//    CPUDOZEbits.DOZE=7; //1:256
//    CPUDOZEbits.ROI = 1;
//    CPUDOZEbits.DOZEN = 1;
//    
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

