

#include "i2c_regs_data.h"
#include "system.h"
#include "timers.h"

typedef enum {
    PI_RUN_HB_INITIAL_DETECT,
    PI_RUN_HB_NORMAL_MONITORING,
} PI_MON_STATE;

volatile PI_MON_STATE pi_mon_state = PI_RUN_HB_INITIAL_DETECT;

#define PI_RUN_HB_NORMAL_MONITORING_TIME_MS 3000
#define PI_RUN_HB_INITIAL_DETECT_TIME_MS 3000

//pi_run_changed flag shows that the PI_RUN pin has changed during the last monitoring period
volatile bool pi_run_changed = false;
static void PI_RUN_PinNegativeEdgeCb(void) { pi_run_changed = true; }

static void PI_RUN_MonitorTimerCallback(volatile struct TaskDescr* taskd) {
    switch (pi_mon_state) {
        case PI_RUN_HB_INITIAL_DETECT:
            if (pi_run_changed) {
                // go to normal state
                // pi run on
                REG_SET_PI_HB_OK();
            } else {
                // pi run off
                // do bat check
                REG_CLEAR_PI_HB_OK();
                add_task(TASK_CHECK_BAT_AVAIL, PowMgrEnableDisableCharging,
                         NULL);
            }
            pi_mon_state = PI_RUN_HB_INITIAL_DETECT;
            RegisterTimerCallback(&PI_RUN_MonitorTimerCallback, TASK_PI_MONITOR,
                                  PI_RUN_HB_NORMAL_MONITORING_TIME_MS,
                                  TIMER_CB_PI_MONITOR);
            EnableTimerCallback(TIMER_CB_PI_MONITOR);

            break;
        case PI_RUN_HB_NORMAL_MONITORING: {
            if (pi_run_changed) {
                // everything ok
                REG_SET_PI_HB_OK();
            } else {
                // pi is not running
                REG_CLEAR_PI_HB_OK();
            }
        }
    }
    pi_run_changed = false;
}

void PI_RUN_MonitorInit(void) {
    //clear pi running by default
    REG_CLEAR_PI_HB_OK();

    pi_run_changed = false;

    // register PI_RUN pin negative edge callback
    Plat_GPIO_Register_PI_RUN_Callback(PI_RUN_PinNegativeEdgeCb);

    // check if pi is running
    uint8_t v1 = PI_RUN_GetValue();
    DelayMS(30);
    uint8_t v2 = PI_RUN_GetValue();

    // if pi is running -> start normal monitor
    // if pi is not running -> start init monitor
    if (v1 || v2) {
        // pi is up and running
        REG_SET_PI_HB_OK();
        // start pi normal monitor
        pi_mon_state = PI_RUN_HB_NORMAL_MONITORING;
        RegisterTimerCallback(&PI_RUN_MonitorTimerCallback, TASK_PI_MONITOR,
                              PI_RUN_HB_NORMAL_MONITORING_TIME_MS,
                              TIMER_CB_PI_MONITOR);
        EnableTimerCallback(TIMER_CB_PI_MONITOR);
    } else {
        // start pi init monitor
        pi_mon_state = PI_RUN_HB_INITIAL_DETECT;
        RegisterTimerCallback(&PI_RUN_MonitorTimerCallback, TASK_PI_MONITOR,
                              PI_RUN_HB_INITIAL_DETECT_TIME_MS,
                              TIMER_CB_PI_MONITOR);
        EnableTimerCallback(TIMER_CB_PI_MONITOR);
    }
}