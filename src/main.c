#include "main.h"

#include "gpio.h"
#include "i2c1_multimode.h"
#include "i2c_app.h"
#include "i2c_regs_data.h"
#include "led_ctrl.h"
#include "onoff.h"
#include "pi_mgr.h"
#include "power_mgr.h"
#include "pwm.h"
#include "system.h"
#include "tasks.h"
#include "timers.h"


void OnOffSwithcPressed(enum ONOFFTypes type) {
    switch (type) {
        case BTN_1L:
            add_task(TASK_PI_SHUTDOWN_OR_WAKEUP,PIShutdownOrWakeup,NULL);
            
            break;
        case BTN_1S_1L:
            //wake up pi
//            MCU_INT_N_SetHigh();
//            if (I2C1_Current_Mode() == I2C1_HOST_MODE) {
//                add_task(TASK_I2C_WAKEUP, read_device_id, regAddrBuff2);
//            }
//            I2C_SEL_N_Toggle();
            break;
        case BTN_1S_1S_1L:
//            add_task(TASK_POWER_IC_SYSTEM_RESET,PowMgrSystemReset,NULL);
            
            break;
    };

}

int main() {
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

    //timers
    BQQON_Sampling_Initialize();
    
    PWM1_16BIT_Initialize();
    

    //comm
    I2C1_Host_Initialize();

    // register GPIO callbacks
    // GPIO_Register_RTC_IRQ_N_Callback(RTCPinChanged);

    PWM1_16BIT_Period_SetInterruptHandler(LED_UpdateCallback);

    // 1msec freerunngin timer irq
    TMR1_OverflowCallbackRegister(MiliSecTimerOverflow);
    TASKS_Initialize();

    
    
    // I2CSwitchMode(I2C1_HOST_MODE);

    //
    // logic inits
    //
    //led manager
    LEDSetPattern(&sleep_pattern);
    // pi monitor
    PI_RUN_MonitorInit();
    // power manager
    PowMgrEnableDisableCharging(NULL);
    // onoff button init
    ONOFF_Initialize();
    ONOFF_CallbackRegister(OnOffSwithcPressed);

    extern int read_ibat;
    void PowMgrReadIBAT(volatile  TaskDescr* taskd);
    PI_5V_DISA_SetLow();//enable pi 
   while(0){ 
    MCU_INT_N_SetLow();
    DelayMS(1000);
    MCU_INT_N_SetHigh();
    DelayMS(1000);
//        PI_5V_DISA_SetHigh();
////        GPIO_Register_BQ_INT_Callback(BQ_INT_PinChanged);
//        PowMgrMesIBATOneShot();
//        DelayMS(1000);
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

#if 0
    
    I2C1_Multi_Initialize();

    // If using interrupts in PIC18 High/Low Priority Mode you need to enable the Global High and Low Interrupts 
    // If using interrupts in PIC Mid-Range Compatibility Mode you need to enable the Global Interrupts 
    // Use the following macros to:

    TASKS_Initialize();
    ONOFF_Initialize();

    ONOFF_CallbackRegister(OnOffSwithcPressed);
    
    //add rtc irq handler
    RTC_IRQ_N_SetInterruptHandler(RTCPinChanged);

    //enable PI RUN pin irq
    SET_PI_HB_NOT_OK(); //by default no HB recorded
    PI_RUN_SetInterruptHandler(PIRunModeChanged);
    
    // Enable the Global Interrupts 
    INTERRUPT_GlobalInterruptHighEnable();
    INTERRUPT_GlobalInterruptLowEnable();
    // Disable the Global Interrupts 
    //INTERRUPT_GlobalInterruptDisable();
    
    //1msec freerunngin timer irq
    TMR1_OverflowCallbackRegister(MiliSecTimerOverflow);
    
    //set charge enable when battery is present
    PowMgrEnableDisableCharging();    
    
    //led control setup
    //set led 0 by default
    PWM1_16BIT_Period_SetInterruptHandler(LED_UpdateCallback);
    LEDSetValue(0);
   
    run_tasks();
    return 0;
#endif
    return 0;
}