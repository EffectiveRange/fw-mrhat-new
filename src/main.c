#include "system.h"
#include "i2c1_multimode.h"

#include "tasks.h"
#include "onoff.h"
#include "power_mgr.h"
#include "main.h"
#include "timers.h"
#include "i2c_app.h"
#include "i2c_regs_data.h"
#include "led_ctrl.h"
#include "pi_mgr.h"
// #include "hello.h"

void GPIO_Init(){
     /**
    SLRCONx registers
    */
    SLRCONA = 0x37;
    SLRCONB = 0xE0;
    SLRCONC = 0xFB;

    /**
    INLVLx registers
    */
    INLVLA = 0x3F;
    INLVLB = 0xE0;
    INLVLC = 0xFB;

    /**
    ANSELx registers
    */
    // ANSELA = 0x3;
    // ANSELB = 0x0;
    // ANSELC = 0x0;

    /**
    WPUx registers
    */
    // WPUA = 0x10;
    // WPUB = 0x0;
    // WPUC = 0x0;

    /**
    ODx registers
    */
    // ODCONA = 0x10;
    // ODCONB = 0xE0;
    // ODCONC = 0xB9;

    /**
    RxyI2C | RxyFEAT registers   
    */
    // RC0FEAT = 0x20;
    // RC1FEAT = 0x20;
    // RC4FEAT = 0x0;
    // RC5FEAT = 0x0;
    // RB5FEAT = 0x21;
    // RB6FEAT = 0x21;

    //outputs

    // portc 0 power loed digit out
    #define PWR_LED_CTRL_TRIS                 TRISCbits.TRISC0
    #define PWR_LED_CTRL_LAT                  LATCbits.LATC0
    #define PWR_LED_CTRL_PORT                 PORTCbits.RC0
    #define PWR_LED_CTRL_WPU                  WPUCbits.WPUC0
    #define PWR_LED_CTRL_OD                   ODCONCbits.ODCC0
    #define PWR_LED_CTRL_ANS                  ANSELCbits.
    #define PWR_LED_CTRL_SetHigh()            do { LATCbits.LATC0 = 1; } while(0)
    #define PWR_LED_CTRL_SetLow()             do { LATCbits.LATC0 = 0; } while(0)
    #define PWR_LED_CTRL_Toggle()             do { LATCbits.LATC0 = ~LATCbits.LATC0; } while(0)
    #define PWR_LED_CTRL_GetValue()           PORTCbits.RC0
    #define PWR_LED_CTRL_SetDigitalInput()    do { TRISCbits.TRISC0 = 1; } while(0)
    #define PWR_LED_CTRL_SetDigitalOutput()   do { TRISCbits.TRISC0 = 0; } while(0)
    #define PWR_LED_CTRL_SetPullup()          do { WPUCbits.WPUC0 = 1; } while(0)
    #define PWR_LED_CTRL_ResetPullup()        do { WPUCbits.WPUC0 = 0; } while(0)
    #define PWR_LED_CTRL_SetPushPull()        do { ODCONCbits.ODCC0 = 0; } while(0)
    #define PWR_LED_CTRL_SetOpenDrain()       do { ODCONCbits.ODCC0 = 1; } while(0)
    #define PWR_LED_CTRL_SetAnalogMode()      do { ANSELCbits. = 1; } while(0)
    #define PWR_LED_CTRL_SetDigitalMode()     do { ANSELCbits. = 0; } while(0)

    #define PWR_LED_CTRL_SetSlewRate()     do { SLRCONCbits.SLRC0 = 1; } while(0)

    PWR_LED_CTRL_SetDigitalOutput();
    PWR_LED_CTRL_SetHigh();
    PWR_LED_CTRL_SetOpenDrain();
    PWR_LED_CTRL_SetSlewRate();


    //portc 1 input rising edge
    #define BQ_QON_N_TRIS                 TRISCbits.TRISC1
    #define BQ_QON_N_LAT                  LATCbits.LATC1
    #define BQ_QON_N_PORT                 PORTCbits.RC1
    #define BQ_QON_N_WPU                  WPUCbits.WPUC1
    #define BQ_QON_N_OD                   ODCONCbits.ODCC1
    #define BQ_QON_N_ANS                  ANSELCbits.
    #define BQ_QON_N_SetHigh()            do { LATCbits.LATC1 = 1; } while(0)
    #define BQ_QON_N_SetLow()             do { LATCbits.LATC1 = 0; } while(0)
    #define BQ_QON_N_Toggle()             do { LATCbits.LATC1 = ~LATCbits.LATC1; } while(0)
    #define BQ_QON_N_GetValue()           PORTCbits.RC1
    #define BQ_QON_N_SetDigitalInput()    do { TRISCbits.TRISC1 = 1; } while(0)
    #define BQ_QON_N_SetDigitalOutput()   do { TRISCbits.TRISC1 = 0; } while(0)
    #define BQ_QON_N_SetPullup()          do { WPUCbits.WPUC1 = 1; } while(0)
    #define BQ_QON_N_ResetPullup()        do { WPUCbits.WPUC1 = 0; } while(0)
    #define BQ_QON_N_SetPushPull()        do { ODCONCbits.ODCC1 = 0; } while(0)
    #define BQ_QON_N_SetOpenDrain()       do { ODCONCbits.ODCC1 = 1; } while(0)
    #define BQ_QON_N_SetAnalogMode()      do { ANSELCbits. = 1; } while(0)
    #define BQ_QON_N_SetDigitalMode()     do { ANSELCbits. = 0; } while(0)

    BQ_QON_N_SetDigitalInput();
    //if needed inrq
    // RC1->INTERRUPT MANAGER:INT0;
    // INT0PPS = 0x11; //RC1->INTERRUPT MANAGER:INT0;
    //no irq for ioc 1



    // get/set RA2 aliases
    //PA 0 RTC irq
    #define RTC_IRQ_N_TRIS                 TRISAbits.TRISA2
    #define RTC_IRQ_N_LAT                  LATAbits.LATA2
    #define RTC_IRQ_N_PORT                 PORTAbits.RA2
    #define RTC_IRQ_N_WPU                  WPUAbits.WPUA2
    #define RTC_IRQ_N_OD                   ODCONAbits.ODCA2
    #define RTC_IRQ_N_ANS                  ANSELAbits.ANSELA2
    #define RTC_IRQ_N_SetHigh()            do { LATAbits.LATA2 = 1; } while(0)
    #define RTC_IRQ_N_SetLow()             do { LATAbits.LATA2 = 0; } while(0)
    #define RTC_IRQ_N_Toggle()             do { LATAbits.LATA2 = ~LATAbits.LATA2; } while(0)
    #define RTC_IRQ_N_GetValue()           PORTAbits.RA2
    #define RTC_IRQ_N_SetDigitalInput()    do { TRISAbits.TRISA2 = 1; } while(0)
    #define RTC_IRQ_N_SetDigitalOutput()   do { TRISAbits.TRISA2 = 0; } while(0)
    #define RTC_IRQ_N_SetPullup()          do { WPUAbits.WPUA2 = 1; } while(0)
    #define RTC_IRQ_N_ResetPullup()        do { WPUAbits.WPUA2 = 0; } while(0)
    #define RTC_IRQ_N_SetPushPull()        do { ODCONAbits.ODCA2 = 0; } while(0)
    #define RTC_IRQ_N_SetOpenDrain()       do { ODCONAbits.ODCA2 = 1; } while(0)
    #define RTC_IRQ_N_SetAnalogMode()      do { ANSELAbits.ANSELA2 = 1; } while(0)
    #define RTC_IRQ_N_SetDigitalMode()     do { ANSELAbits.ANSELA2 = 0; } while(0)
    #define RA2_SetInterruptHandler  RTC_IRQ_N_SetInterruptHandler
    //
    RTC_IRQ_N_SetDigitalInput();
    IOCAFbits.IOCAF2 = 0;
    // RA2->INTERRUPT MANAGER:INT1;
    IOCANbits.IOCAN2 = 1; //falling edge irq enable
    IOCAPbits.IOCAP2 = 0; //rising edge irq enable
    // set irq fun
    INT1PPS = 0x02; //RC1->INTERRUPT MANAGER:INT0;



    // get/set RC6 aliases
    #define PI_RUN_TRIS                 TRISCbits.TRISC6
    #define PI_RUN_LAT                  LATCbits.LATC6
    #define PI_RUN_PORT                 PORTCbits.RC6
    #define PI_RUN_WPU                  WPUCbits.WPUC6
    #define PI_RUN_OD                   ODCONCbits.ODCC6
    #define PI_RUN_ANS                  ANSELCbits.ANSELC6
    #define PI_RUN_SetHigh()            do { LATCbits.LATC6 = 1; } while(0)
    #define PI_RUN_SetLow()             do { LATCbits.LATC6 = 0; } while(0)
    #define PI_RUN_Toggle()             do { LATCbits.LATC6 = ~LATCbits.LATC6; } while(0)
    #define PI_RUN_GetValue()           PORTCbits.RC6
    #define PI_RUN_SetDigitalInput()    do { TRISCbits.TRISC6 = 1; } while(0)
    #define PI_RUN_SetDigitalOutput()   do { TRISCbits.TRISC6 = 0; } while(0)
    #define PI_RUN_SetPullup()          do { WPUCbits.WPUC6 = 1; } while(0)
    #define PI_RUN_ResetPullup()        do { WPUCbits.WPUC6 = 0; } while(0)
    #define PI_RUN_SetPushPull()        do { ODCONCbits.ODCC6 = 0; } while(0)
    #define PI_RUN_SetOpenDrain()       do { ODCONCbits.ODCC6 = 1; } while(0)
    #define PI_RUN_SetAnalogMode()      do { ANSELCbits.ANSELC6 = 1; } while(0)
    #define PI_RUN_SetDigitalMode()     do { ANSELCbits.ANSELC6 = 0; } while(0)
    PI_RUN_SetDigitalMode();
    PI_RUN_SetDigitalInput();
    IOCCFbits.IOCCF6 = 0;
    IOCCNbits.IOCCN6 = 1; //falling edge irq enable
    IOCCPbits.IOCCP6 = 1; //rising edge irq enable

    // return;
    IOCAP = 0x0;
    IOCAN = 0x4;
    IOCAF = 0x0;
    IOCWP = 0x0;
    IOCWN = 0x0;
    IOCWF = 0x0;
    IOCBP = 0x0;
    IOCBN = 0x0;
    IOCBF = 0x0;
    IOCCP = 0x0;
    IOCCN = 0xC0;
    IOCCF = 0x0;
}
void RTC_IRQ_N_Isr(void){
    
    
}
void BQ_INT_N_Isr(){

}
void PI_RUN_Isr(){
    PWR_LED_CTRL_Toggle();
}
void PIN_MANAGER_IOC(void){
    if(IOCAFbits.IOCAF2 == 1){
        RTC_IRQ_N_Isr();
        IOCAFbits.IOCAF2 = 0;
    }
    else if(IOCCFbits.IOCCF6 == 1){
        PI_RUN_Isr();
        IOCCFbits.IOCCF6 = 0;
    }
} 

int main() {
    // hello();
    // while(1);
    
    SYSTEM_Initialize();
    PIE3bits.IOCIE = 1;

    

    
    INTERRUPT_GlobalInterruptHighEnable();
    INTERRUPT_GlobalInterruptLowEnable();
    
    // PWR_LED_CTRL_SetDigitalOutput();
    // PWR_LED_CTRL_SetHigh();
    // PWR_LED_CTRL_SetLow();
    while(1){
        __delay_ms(500);
//         PWR_LED_CTRL_Toggle() ;
        // __delay_ms(1000);

    }
    return 0;
}