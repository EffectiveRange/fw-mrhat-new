
#include "power_mgr.h"

#include "bits.h"
#include "i2c1.h"
#include "i2c_app.h"
#include "i2c_regs_data.h"
#include "led_ctrl.h"
#include "system.h"
#include "tasks.h"
#include "timers.h"

extern volatile uint8_t CLIENT_DATA[];

int ReadPartId(){
    uint8_t val8;
    // test i2c read, if could not read partid do not check battery return
    uint8_t i = 0;
    for (i = 0; i < 10; i++) {
        // raead partid
        I2CReadByte(BQ_I2C_ADDR, 0x38, &val8);
        if (GET_BIT(val8, 3) != 0) {
            // partid is read
            return 0;
        }
        DelayMS(10);
    }

    if (GET_BIT(val8, 3) == 0) {
        return -1;
    }
    return 0;
}
void PowMgrEnableDisableCharging(volatile TaskDescr* taskd) {
    uint8_t tx[2];
    uint8_t rx[2];
    uint8_t val8;
    uint8_t val16;
    int ret = 0;

    if(ReadPartId()!=0){
        return;
    }

    // enable Force a battery discharging current (~30mA)
    // reset watchdog
    // REG0x16_Charger_Control_1 Register,
    // BIT6 FORCE_IBATDIS - force current on battery
    // BIT2 WATCHDOG reset
    ret += I2CReadByte(BQ_I2C_ADDR, 0x16, &val8);
    SET_BIT(val8, 6);  // BIT6 FORCE_IBATDIS
    SET_BIT(val8, 2);  // BIT2 WATCHDOG reset
    ret += I2CWriteByte(BQ_I2C_ADDR, 0x16, val8);

    // little delay for discharge current, I know it has a speed of light :D
    DelayMS(100);

    // enable ADC
    // REG0x26_ADC_Control Register
    // BIT7 ADC_EN
    ret += I2CReadByte(BQ_I2C_ADDR, 0x26, &val8);
    SET_BIT(val8, 7);  // BIT7 ADC_EN
    ret += I2CWriteByte(BQ_I2C_ADDR, 0x26, val8);

    // adc sample takes 24milisec
    DelayMS(100);
    // read ADC
    // REG0x30_VBAT_ADC Register bits 1:12, read 2bytes
    tx[0] = 0x30;
    ret += I2CWriteRead(BQ_I2C_ADDR, tx, 1, rx, 2);

    // convert adc to mV
    uint16_t adc_bits =
        (uint16_t)(rx[0] + (rx[1] << 8));  // value is little endian
    adc_bits &= (0x1ffe);
    adc_bits = adc_bits >> 1;
    float adc_mV = adc_bits * 1.99f;  // mili Volt

    // disable ADC
    // REG0x26_ADC_Control Register
    // BIT7 ADC_EN
    ret += I2CReadByte(BQ_I2C_ADDR, 0x26, &val8);
    CLEAR_BIT(val8, 7);  // BIT7 ADC_EN
    ret += I2CWriteByte(BQ_I2C_ADDR, 0x26, val8);

    // disable Force a battery discharging current (~30mA)
    // REG0x16_Charger_Control_1 Register
    // BIT6 FORCE_IBATDIS
    ret += I2CReadByte(BQ_I2C_ADDR, 0x16, &val8);
    CLEAR_BIT(val8, 6);  // BIT6 FORCE_IBATDIS
    ret += I2CWriteByte(BQ_I2C_ADDR, 0x16, val8);

    // VSYSMIN value is ~2500mV under 2000mV there is no battery
    // ADC range can be 0mV-5572mV (0h-AF0h)
    if (ret == 0 && adc_mV > 2000 && adc_bits < 0xAF0) {
        // Charger enable register of bat manager ic
        // REG0x16_Charger_Control_1 Register,
        // BIT5 EN_CHG
        //  1 : enable
        //  0 : disable
        ret += I2CReadByte(BQ_I2C_ADDR, 0x16, &val8);
        SET_BIT(val8, 5);  // BIT5 EN_CHG
        ret += I2CWriteByte(BQ_I2C_ADDR, 0x16, val8);

        // /CE pin of battery management ic is active low
        // CHG_DISA pin=0 -> Charge enable
        CHG_DISA_SetLow();

        // set bit0 of reg2 STAT
        REG_SET_BAT_AVAIL();

    } else {
        // Charger enable register of bat manager ic
        // REG0x16_Charger_Control_1 Register, BIT5 EN_CHG
        //  1 : enable
        //  0 : disable
        ret += I2CReadByte(BQ_I2C_ADDR, 0x16, &val8);
        CLEAR_BIT(val8, 5);  // BIT5 EN_CHG
        ret += I2CWriteByte(BQ_I2C_ADDR, 0x16, val8);

        // /CE pin of battery management ic is active low
        // CHG_DISA pin=1 -> Charge disable
        CHG_DISA_SetHigh();

        // clear bit0 of reg2 STAT
        REG_CLEAR_BAT_AVAIL();
    }
    if (ret != 0) {
        REG_SET_BAT_CHECK_ERR();
    } else {
        REG_CLEAR_BAT_ERR();
    }

    return;
}

// 2-56
// volatile uint8_t tx_all[2];
// volatile uint8_t rx_all[56];
// void ReadAll(void) {
//     int ret = 0;
//     tx_all[0] = 0x2;
//     ret += I2CWriteRead(BQ_I2C_ADDR, tx_all, 1, &rx_all[2], 54);
// }
volatile int16_t ibat_signed = 0;
volatile int v0, v6;
void PowMgrReadIBAT(volatile TaskDescr* taskd) {
    uint8_t tx[2];
    uint8_t rx[2];
    uint8_t val8;
    uint8_t val16;
    int ret = 0;

    rm_task(TASK_CHECK_BQ_IRQ);

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
}
void BQ_INT_PinChanged(void) {
    if (BQ_INT_N_GetValue()) {
        // rising edge
        return;
    }
    // fallin edege
    add_task(TASK_CHECK_BQ_IRQ, PowMgrReadIBAT, NULL);
}
// Start IBAT ADC measurement
void PowMgrStartMesIBAT(volatile TaskDescr* taskd) {
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

    //BIT 1:0 WATCHDOG watchdog timer setting
    // 0h = Disable
    // 1h = 50s (default)
    // 2h = 100s
    // 3h = 200s
    CLEAR_BIT(val8, 1);
    SET_BIT(val8, 0);

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
    //  bit3:running average,
    //  bit2:start new conversion
    // bit4:5 ADC_SAMPLE=3 (9bit)
    SET_BIT(val8, 7);  // BIT7 ADC_EN
    SET_BIT(val8, 3);  // BIT3 running average
    SET_BIT(val8, 2);  // BIT2 start new conversion

    CLEAR_BIT(val8, 5);  // BIT5 ADC_SAMPLE 12bit
    CLEAR_BIT(val8, 4);  // BIT4 ADC_SAMPLE 12bit

    // SET_BIT(val8, 4);  // BIT4 ADC_SAMPLE 9bit
    // SET_BIT(val8, 5);  // BIT5 ADC_SAMPLE 9bit

    // clear bits
    // bit6:ADC_RATE=0 (continous mes)
    CLEAR_BIT(val8, 6);  // BIT6 ADC_RATE
    ret += I2CWriteByte(BQ_I2C_ADDR, 0x26, val8);

    // enable bq int handler
    GPIO_Register_BQ_INT_Callback(BQ_INT_PinChanged);
}

int PowMgrMesIBATOneShot(void) {
    uint8_t tx[2];
    uint8_t rx[2];
    uint8_t val8;
    uint8_t val16;

    int ret = 0;

    // reset watchdog
    // #REG0x16_Charger_Control_1 Register,
    // BIT2 WATCHDOG reset
    tx[0] = 0x16;
    ret += I2CReadByte(BQ_I2C_ADDR, 0x16, &val8);
    SET_BIT(val8, 2);  // BIT2 WATCHDOG reset
    ret += I2CWriteByte(BQ_I2C_ADDR, 0x16, val8);

    // REG0x27_ADC_Function_Disable_0 Registe
    //    bit6: IBAT_ADC_DIS , BAT ADC control
    //      0h = Enable (Default) 1h = Disable
    tx[0] = 0x27;
    ret += I2CReadByte(BQ_I2C_ADDR, 0x27, &val8);
    CLEAR_BIT(val8, 6);  // BIT6 IBAT_ADC_DIS
    ret += I2CWriteByte(BQ_I2C_ADDR, 0x27, val8);

    // REG0x26_ADC_Control Register
    ret += I2CReadByte(BQ_I2C_ADDR, 0x26, &val8);
    // set bits
    // bit7:ADC_EN=enable,
    // bit3:running average,
    // bit2:start new conversion
    // bit4:5 ADC_SAMPLE=0 (12bit)
    SET_BIT(val8, 7);  // BIT7 ADC_EN
    SET_BIT(val8, 6);  // BIT6 one shot value=0

    CLEAR_BIT(val8, 5);  // BIT5 ADC_SAMPLE 12bit
    CLEAR_BIT(val8, 4);  // BIT4 ADC_SAMPLE 12bit

    // SET_BIT(val8, 5);    // BIT5 ADC_SAMPLE 9bit
    // SET_BIT(val8, 4);    // BIT4 ADC_SAMPLE 9bit

    CLEAR_BIT(val8, 3);  // BIT3 single value
    SET_BIT(val8, 2);    // BIT2 start new conversion
    ret += I2CWriteByte(BQ_I2C_ADDR, 0x26, val8);

    DelayMS(100);
    // read ibat adc
    // REG0x2A_IBAT_ADC Register
    tx[0] = 0x2A;
    ret += I2CWriteRead(BQ_I2C_ADDR, tx, 1, rx, 2);
    CLIENT_DATA[REG_IBAT_ADDR] = rx[0];
    CLIENT_DATA[REG_IBAT_ADDR + 1] = rx[1];
    ibat_signed = (int16_t)((uint16_t)(rx[1] << 8) | rx[0]);

    return ret;
}

#if 0
void PowMgrSystemReset(volatile  TaskDescr* taskd){
    
    uint8_t tx[2];
    uint8_t rx[2];
    int ret=0;
    tx[0]=0x18;
    
     
    //Trigger power IC system reset
    //#REG0x18_Charger_Control_3 Register
    //BIT3 BATFET_CTRL_WV_BUS -allows batfet off or system power reset with adapter present.
    //0->disable
    //1->enable
    
    //BIT2 BATFET_DLY - Delay time added to the taking action in bits [1:0] of the BATFET_CTRL
    //0x0 = Add 20 ms delay time
    //0x1 = Add 10s delay time (default)
    
    //BIT0:1 BATFET_CTRL
    //The control logic of the BATFET to force the device enter different modes.
    //0x0 = Normal (default)
    //0x1 = Shutdown Mode
    //0x2 = Ship Mode
    //0x3 = System Power Reset
    
    ret += I2CWriteRead(0x6b, tx,1, rx, 1);
    //enable BATFET_CTRL_WVBUS
    //go to ship mode
    tx[1] = rx[0] | (0x3) | (1<<3) ;
    //set delay to 20ms
    tx[1] = tx[1] & ~(1<<2);
    //default it takes 10sec to go to ship mode after request
    ret += I2CWrite(BQ_I2C_ADDR, tx, 2);
    
    //here the POWER IC should be reseted
    
    rm_task(TASK_POWER_IC_SYSTEM_RESET);

//    return ret;
}
#endif

#if 0
int PowMgrGoToShipMode(void){
        
    uint8_t tx[2];
    uint8_t rx[2];
    int ret=0;
    tx[0]=0x18;
    
    
    //Trigger power IC go to ship mode
    //#REG0x18_Charger_Control_3 Register
    //BIT3 BATFET_CTRL_WV BUS -allows BATFET off or system power reset with adapter present.
    //0->disable
    //1->enable
    //BIT0:1 BATFET_CTRL
    //The control logic of the BATFET to force the device enter different modes.
    //0x0 = Normal (default)
    //0x1 = Shutdown Mode
    //0x2 = Ship Mode
    //0x3 = System Power Reset
    
    ret += I2CWriteRead(BQ_I2C_ADDR, tx,1, rx, 1);
    //enable BATFET_CTRL_WVBUS
    //go to ship mode
    tx[1] = rx[0] | (0x2) | (1<<3) ;
    //default it takes 10sec to go to ship mode after request
    ret += I2CWrite(BQ_I2C_ADDR, tx, 2);
    return ret;
    
}

#endif