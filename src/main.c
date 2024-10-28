#include "main.h"

#include "gpio.h"
#include "i2c1_multimode.h"
#include "i2c_app.h"
#include "i2c_client_types.h"
#include "i2c_regs_data.h"
#include "led_ctrl.h"
#include "onoff.h"
#include "pi_mgr.h"
#include "power_mgr.h"
#include "pwm.h"
#include "stdbool.h"
#include "system.h"
#include "tasks.h"
#include "timers.h"

void OnOffSwithcPressed(enum ONOFFTypes type) {
    switch (type) {
        case BTN_1L:
            add_task(TASK_PI_SHUTDOWN_OR_WAKEUP, PIShutdownOrWakeup, NULL);

            break;
        case BTN_1S_1L:
            // wake up pi
            //            MCU_INT_N_SetHigh();
            //            if (I2C1_Current_Mode() == I2C1_HOST_MODE) {
            //                add_task(TASK_I2C_WAKEUP, read_device_id,
            //                regAddrBuff2);
            //            }
            //            I2C_SEL_N_Toggle();
            break;
        case BTN_1S_1S_1L:
            //            add_task(TASK_POWER_IC_SYSTEM_RESET,PowMgrSystemReset,NULL);

            break;
    };
}
extern void lowpowertest();
int main() {
    // lowpowertest();
    // todo move them
    /**
    ANSELx registers
    */
    ANSELA = 0x3;
    ANSELB = 0x0;
    ANSELC = 0x0;
    // end todo

    // platform inits
    CLOCK_Initialize();
    CPU_Initialize();
    Timer1_Initialize();
    INTERRUPT_Initialize();
    INTERRUPT_GlobalInterruptHighEnable();
    INTERRUPT_GlobalInterruptLowEnable();
    GPIO_Init();
    GPIO_Interrupt_Init();
    MVIO_Initialize();

    // timers
    BQQON_Sampling_Initialize();

    PWM1_16BIT_Initialize();

    // comm

    I2C1_Host_Initialize();
    I2C1_Multi_Initialize();
    I2CSwitchMode(I2C1_HOST_MODE);
    I2CSwitchMode(I2C1_CLIENT_MODE);

    // register GPIO callbacks
    // GPIO_Register_RTC_IRQ_N_Callback(RTCPinChanged);

    PWM1_16BIT_Period_SetInterruptHandler(LED_UpdateCallback);

    // 1msec freerunngin timer irq
    TMR1_OverflowCallbackRegister(MiliSecTimerOverflow);
    TASKS_Initialize();

    //
    // logic inits
    //
    // led manager
    LEDSetPattern(&sleep_pattern);
    // pi monitor
    PI_RUN_MonitorInit();
    // PowMgrEnableDisableCharging(NULL);
    // onoff button init
    ONOFF_Initialize();
    ONOFF_CallbackRegister(OnOffSwithcPressed);

    GPIO_Register_RTC_IRQ_N_Callback(RTCPinChanged);
    PI_5V_DISA_SetLow();  // enable PI

    // power manager
    ReadPartId();
    while (0) {
        PI_5V_DISA_SetHigh();
        // MCU_INT_N_SetLow();
        // DelayMS(1000);
        // MCU_INT_N_SetHigh();
        DelayMS(1000);
        //        PI_5V_DISA_SetHigh();
        GPIO_Register_BQ_INT_Callback(BQ_INT_PinChanged);
        PowMgrMesIBATOneShot();
        DelayMS(1000);
        ////        PowMgrReadIBAT(NULL);
        ////        if(read_ibat){
        ////            PowMgrMesIBAT();
        ////        }
        ////        DelayMS(3000);
        ////         PI_5V_DISA_SetLow();
        ////        DelayMS(3000);
        //
    }

    run_tasks();
    while (1);

    return 0;
}