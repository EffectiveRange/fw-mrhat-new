#include "main.h"

#include "i2c1_multimode.h"
#include "i2c_app.h"
#include "i2c_regs_data.h"
#include "led_ctrl.h"
#include "onoff.h"
#include "pi_mgr.h"
#include "plat_gpio.h"
#include "plat_pwm.h"
#include "power_mgr.h"
#include "system.h"
#include "tasks.h"
#include "timers.h"
#include "pi_mgr.h"
#include "i2c_regs_data.h"

int main() {
    /**
    ANSELx registers
    */
    ANSELA = 0x3;
    ANSELB = 0x0;
    ANSELC = 0x0;
    // inits
    SYSTEM_Initialize();
    Plat_GPIO_Init();
    Plat_GPIO_Interrupt_Init();
    Plat_PWM1_16BIT_Initialize();

    // register GPIO callbacks
    // Plat_GPIO_Register_RTC_IRQ_N_Callback(RTCPinChanged);

    LEDSetPattern(&sleep_pattern);
    Plat_PWM1_16BIT_Period_SetInterruptHandler(LED_UpdateCallback);

    // 1msec freerunngin timer irq
    TMR1_OverflowCallbackRegister(MiliSecTimerOverflow);
    I2C1_Multi_Initialize();
    INTERRUPT_GlobalInterruptHighEnable();
    INTERRUPT_GlobalInterruptLowEnable();

    //logic inits
    PI_RUN_MonitorInit();
    TASKS_Initialize();
    I2CSwitchMode(I2C1_HOST_MODE);
    PowMgrEnableDisableCharging();
    while(1);
    while (1) {
        // timer test
        REG_SET_SHUTDOWN_REQ();
        REG_SET_SHUTDOWN_REQ() ;
        REG_CLEAR_SHUTDOWN_REQ() ;
        REG_IS_SHUTDOWN_REQ();
        REG_SET_PI_HB_OK() ;
        REG_CLEAR_PI_HB_OK() ;
        REG_IS_PI_HB_OK() ;
        REG_SET_BAT_AVAIL() ;
        REG_CLEAR_BAT_AVAIL();
        REG_IS_BAT_AVAIL() ;
        REG_SET_BAT_CHECK_ERR() ;
        REG_CLEAR_BAT_ERR();
        REG_IS_BAT_CHECK_ERR() ;


    }

    while (1) {
        // LEDSetValue(0x00);
        // __delay_ms(500);
        // LEDSetValue(1);
        // __delay_ms(500);
        // for (uint16_t i = 0 ; i< 100; i+=10){
        //     //100*50 msec = 5 sec
        //     Plat_PWM_DutyCycleSetPercentage_Slice1(i);
        //     __delay_ms(100);
        // }
        // for (uint16_t i = 100 ; i>0; i-=10){
        //     //100*50 msec = 5 sec
        //     Plat_PWM_DutyCycleSetPercentage_Slice1(i);
        //     __delay_ms(100);
        // }
    }

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