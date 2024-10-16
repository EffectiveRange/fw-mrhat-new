

#include "i2c_regs_data.h"
#include "system.h"
#include "timers.h"
#include "power_mgr.h"
#include "i2c_app.h"
#include "led_ctrl.h"

typedef enum {
    PI_RUN_HB_INITIAL_DETECT,
    PI_RUN_HB_NORMAL_MONITORING,
} PI_MON_STATE;

volatile PI_MON_STATE pi_mon_state = PI_RUN_HB_INITIAL_DETECT;

#define PI_RUN_HB_NORMAL_MONITORING_TIME_MS 3000
#define PI_RUN_HB_INITIAL_DETECT_TIME_MS 3000

//pi_run_changed flag shows that the PI_RUN pin has changed during the last monitoring period
volatile bool pi_run_changed = false;
static void PI_RUN_PinNegativeEdgeCb(void) { 
    pi_run_changed = true;
    
     }

static bool pi_running = false;
void PI_ChangeState(bool state) {
    if(state != pi_running){
        pi_running = state;
        if(state){
            REG_SET_PI_HB_OK();
            LEDSetToggleTime(100);
        }else{
            REG_CLEAR_PI_HB_OK();
            LEDSetPattern(&sleep_pattern);
        }
    }
}
static void PI_RUN_MonitorTimerCallback(volatile TaskDescr* taskd) {
    //rm myself
    rm_task(TASK_PI_MONITOR);

    if(pi_run_changed){
        PI_ChangeState(true);
    }else{
        PI_ChangeState(false);
    }
    switch (pi_mon_state) {
        case PI_RUN_HB_INITIAL_DETECT:
            if (pi_run_changed) {
                // go to normal state
                // pi run on
                // REG_SET_PI_HB_OK();
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
            // if (pi_run_changed) {
            //     // everything ok
            //     REG_SET_PI_HB_OK();
            // } else {
            //     // pi is not running
            //     REG_CLEAR_PI_HB_OK();
            // }
            RegisterTimerCallback(&PI_RUN_MonitorTimerCallback, TASK_PI_MONITOR,
                                  PI_RUN_HB_NORMAL_MONITORING_TIME_MS,
                                  TIMER_CB_PI_MONITOR);
            EnableTimerCallback(TIMER_CB_PI_MONITOR);
        }
    }
    pi_run_changed = false;
}

void PI_RUN_MonitorInit(void) {
    //clear pi running by default
    REG_CLEAR_PI_HB_OK();

    pi_run_changed = false;

    // register PI_RUN pin negative edge callback
    GPIO_Register_PI_RUN_Callback(PI_RUN_PinNegativeEdgeCb);

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


//button 1L (1 long press calls this)
//if PI is running -> request it to shut down
//if PI is not running -> wakeup PI


void TaskWakeupPI(volatile struct TaskDescr* taskd){
    ER_MODE_0_SetHigh();
    //do one transaction
    uint8_t tx[2];
    uint8_t rx[2];
    int ret=0;
   
    //test i2c read, if could not read partid do not check battery return
    uint8_t i=0;
    for(int i =0;i<10;i++){
        uint8_t val;
        //todo 6b address
        ret += I2CReadByteWithPI(0x6b,0x38, &val);
        //todo refactor
        if((val & (1<<3)) == (1<<3)){
            break;
        }
        DelayMS(10);
        
    }   
    
    DelayMS(100);
    
    //remove myself
    rm_task(TASK_WAKE_UP_PI);
    
}
void SetMCU_INT_Pin(volatile  TaskDescr* taskd){
    //rm myself
    rm_task(TASK_SET_MCU_INT_PIN);
    
    //SET MCU INT high
    MCU_INT_N_SetHigh();
}

void PIShutdownOrWakeup(volatile  TaskDescr* taskd){
    //if PI running
    if(REG_IS_PI_HB_OK()){
        
        //trigger shutdown
        REG_SET_SHUTDOWN_REQ();
        
        //trigger pi interrupt
        //SET MCU INT generate falling edge interrupt for PI
        MCU_INT_N_SetLow();

        RegisterTimerCallback(&SetMCU_INT_Pin, TASK_SET_MCU_INT_PIN,
                              10000,
                              TIMER_CB_WAKEUP_PI);
        EnableTimerCallback(TIMER_CB_WAKEUP_PI);
        
    }else{
        //wakeup PI
         add_task(TASK_WAKE_UP_PI, TaskWakeupPI, NULL);
        
    }
    rm_task(TASK_PI_SHUTDOWN_OR_WAKEUP);
    
}