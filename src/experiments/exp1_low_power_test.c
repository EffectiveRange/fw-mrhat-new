

#include "bits.h"
#include "system.h"
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
volatile int16_t ibat_signed = 0;
volatile int v0, v6;
volatile bool read_ibat = false;
volatile bool start_mes = false;
void ReadIbat(void) {
    uint8_t tx[2];
    uint8_t rx[2];
    uint8_t val8;
    uint8_t val16;
    int ret = 0;
    read_ibat = false;

    // read ibat adc
    // REG0x1D_Charger_Status_0
    tx[0] = 0x1d;
    ret += I2CReadByte(BQ_I2C_ADDR, 0x1d, &val8);

    if ((CHECK_BIT(val8, 0)) || (CHECK_BIT(val8, 6))) {
        // read ibat adc
        // REG0x2A_IBAT_ADC Register
        tx[0] = 0x2A;
        ret += I2CWriteRead(BQ_I2C_ADDR, tx, 1, rx, 2);
        CLIENT_DATA[REG_IBAT_ADDR] = rx[0];
        CLIENT_DATA[REG_IBAT_ADDR + 1] = rx[1];
        ibat_signed = (int16_t)((uint16_t)(rx[1] << 8) | rx[0]);
        LEDSetToggleTime(
            1000);  // todo p0 remove just for debug that the ibat mes happened

        // disable BQ irq handler
        GPIO_Register_BQ_INT_Callback(NULL);
    }
    start_mes=true;
}
void bq_irq(void) {
    if (BQ_INT_N_GetValue()) {
        // rising edge
        return;
    }
    read_ibat = true;
}
// Start IBAT ADC measurement
void StartMesIbat() {
    uint8_t tx[2];
    uint8_t rx[2];
    uint8_t val8;
    uint8_t val16;
    rm_task(TASK_START_MES_IBAT);
    int ret = 0;

    // todo mate p0 remove
    ret += I2CReadByte(BQ_I2C_ADDR, 0x23, &val8);
    CLEAR_BIT(val8, 0);  // BIT2 WATCHDOG irq enable
    ret += I2CWriteByte(BQ_I2C_ADDR, 0x23, val8);
    // end todo

    // reset watchdog
    // #REG0x16_Charger_Control_1 Register,
    // BIT2 WATCHDOG reset
    ret += I2CReadByte(BQ_I2C_ADDR, 0x16, &val8);
    SET_BIT(val8, 2);  // BIT2 WATCHDOG reset

    // BIT 1:0 WATCHDOG watchdog timer setting
    //  0h = Disable
    //  1h = 50s (default)
    //  2h = 100s
    //  3h = 200s
    //50sec
     CLEAR_BIT(val8, 1);
     SET_BIT(val8, 0);
    //100sec
//    SET_BIT(val8, 1);
//    CLEAR_BIT(val8, 0);

    ret += I2CWriteByte(BQ_I2C_ADDR, 0x16, val8);

    // REG0x27_ADC_Function_Disable_0 Register
    //    bit6: IBAT_ADC_DIS , BAT ADC control
    //      0h = Enable (Default) 1h = Disable
    ret += I2CReadByte(BQ_I2C_ADDR, 0x27, &val8);
    CLEAR_BIT(val8, 6);  // BIT6 IBAT_ADC_DIS
    ret += I2CWriteByte(BQ_I2C_ADDR, 0x27, val8);

    // REG0x26_ADC_Control Register
    ret += I2CReadByte(BQ_I2C_ADDR, 0x26, &val8);
    // set bits
    //  bit7:ADC_EN=enable,
    //  bit2:start new conversion
    SET_BIT(val8, 7);  // BIT7 ADC_EN

    //  bit3:running average,
    SET_BIT(val8, 3);  // BIT3 running average
    SET_BIT(val8, 2);  // BIT2 start new conversion

    // bit4:5 ADC_SAMPLE=0 (12bit)
    // CLEAR_BIT(val8, 5);  // BIT5 ADC_SAMPLE 12bit
    // CLEAR_BIT(val8, 4);  // BIT4 ADC_SAMPLE 12bit


    // bit4:5 ADC_SAMPLE=1 (11bit)
    // CLEAR_BIT(val8, 5);  // BIT5 ADC_SAMPLE 12bit
    // SET_BIT(val8, 4);  // BIT4 ADC_SAMPLE 12bit

    // bit4:5 ADC_SAMPLE=2 (10bit)
    // SET_BIT(val8, 5);  // BIT5 ADC_SAMPLE 12bit
    // CLEAR_BIT(val8, 4);  // BIT4 ADC_SAMPLE 12bit

    SET_BIT(val8, 5);  // BIT5 ADC_SAMPLE 9bit
    SET_BIT(val8, 4);  // BIT4 ADC_SAMPLE 9bit

    // clear bits
    // bit6:ADC_RATE=0 (continous mes)
    CLEAR_BIT(val8, 6);  // BIT6 ADC_RATE

    ret += I2CWriteByte(BQ_I2C_ADDR, 0x26, val8);

    // enable bq int handler
    GPIO_Register_BQ_INT_Callback(bq_irq);
}
static void __idletest() ;
void lowpowertest() {
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
    // ONOFF_CallbackRegister(OnOffSwithcPressed);

    GPIO_Register_RTC_IRQ_N_Callback(RTCPinChanged);
    // PI_5V_DISA_SetLow();  // enable PI

    // power manager
    ReadPartId();
    PI_5V_DISA_SetHigh();
    start_mes=true;
    while (1) {
        if (start_mes) {
            StartMesIbat();
            start_mes=false;
        }
        if (read_ibat) {
            ReadIbat();
            read_ibat=false;

        }
        __idletest();
    }

}

static void __idletest() {
    //When the IDLEN bit is clear, the SLEEP instruction will put the device into full Sleep mode. 
    //When IDLEN is set, the SLEEP instruction will put the device into Idle mode. 
    //In Idle mode, the CPU and memory operations are halted, but the peripheral clocks continue to run. 
    //This mode is similar to Doze mode, except that in Idle both the CPU and program memory are shut off.
    
    //   CPUDOZEbits.IDLEN = 1;//go to idle
  CPUDOZEbits.IDLEN = 0; //go to sleep 

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