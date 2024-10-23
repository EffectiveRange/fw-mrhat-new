
#include "pins.h"
#include "system.h"

#include "gpio.h"
void GPIO_Interrupt_Init() {
  // rtc
  RTC_IRQ_N_EnableIOCNegativeEdge();
  // bq int
  BQ_INT_EnableIOCNegativeEdge();
  // pirun
  PI_RUN_EnableIOCNegativeEdge();
  //bq qon
  BQ_QON_N_EnableIOCNegativeEdge();

  //enable peripheriel interrupe enable : IOC(interrupt on change) interrupt enable
  PIE3bits.IOCIE = 1;
}

typedef void (*void_cb_t)(void);

void_cb_t RTC_IRQ_N_Callback = NULL;
void_cb_t BQ_INT_Callback = NULL;
void_cb_t PI_RUN_Callback = NULL;
void_cb_t BQ_QON_Callback = NULL;
void GPIO_Register_RTC_IRQ_N_Callback(void (*callback)(void)) {
  RTC_IRQ_N_Callback = callback;
}
void GPIO_Register_BQ_INT_Callback(void (*callback)(void)) {
  BQ_INT_Callback = callback;
}
void GPIO_Register_PI_RUN_Callback(void (*callback)(void)) {
  PI_RUN_Callback = callback;
}
void GPIO_Register_BQ_QON_Callback(void (*callback)(void)) {
  BQ_QON_Callback = callback;
}
void PIN_MANAGER_IOC(void) {
  // interrupt on change for pin RTC_IRQ_N
  if (IOCAFbits.IOCAF2 == 1) {
    if (RTC_IRQ_N_Callback != NULL) {
      RTC_IRQ_N_Callback();
    }
    IOCAFbits.IOCAF2 = 0;
  }
  // interrupt on change for pin PI_RUN
  if (IOCCFbits.IOCCF6 == 1) {
    if (PI_RUN_Callback != NULL) {
      PI_RUN_Callback();
    }
    IOCCFbits.IOCCF6 = 0;
  }
  // interrupt on change for pin BQ_QON
  if (IOCCFbits.IOCCF1 == 1) {
    if (BQ_QON_Callback != NULL) {
      BQ_QON_Callback();
    }
    IOCCFbits.IOCCF1 = 0;
  }
  // interrupt on change for pin BQ_INT_N
  if (IOCCFbits.IOCCF7 == 1) {
    if (BQ_INT_Callback != NULL) {
      BQ_INT_Callback();
    }
    IOCCFbits.IOCCF7 = 0;
  }
}

void GPIO_Init(void) {
  /**
    SLRCONx registers
    set slew rate for all pins
  */
  SLRCONA = 0x37;
  SLRCONB = 0xE0;
  SLRCONC = 0xFB;

  /**
    INLVLx registers
    The INLVLx register controls the input voltage threshold for each available
    PORTx input pin. A selection between the Schmitt Trigger (ST,
    CMOS-compatible) and the Low-Voltage Buffer (LVBUF, TTL-compatible)
    thresholds is available.
    */
  INLVLA = 0x3F;
  INLVLB = 0xE0;
  INLVLC = 0xFB;

  // BQ_QON_N RC1 input
  BQ_QON_N_SetDigitalMode();
  BQ_QON_N_SetDigitalInput();

  // RTC_IRQ_N RA2 input
  RTC_IRQ_N_SetDigitalMode();
  RTC_IRQ_N_SetDigitalInput();

  // BQ_INT_N RC7 open drain input
  BQ_INT_N_SetDigitalMode();
  BQ_INT_N_SetDigitalInput();
  BQ_INT_N_SetOpenDrain();

  // PI_RUN RC6 input
  PI_RUN_SetDigitalMode();
  PI_RUN_SetDigitalInput();

  // ER_MODE_0 RA4 output
  ER_MODE_0_SetDigitalMode();
  ER_MODE_0_SetDigitalOutput();
  ER_MODE_0_SetPullup();
  ER_MODE_0_SetOpenDrain();

  // MCU_INT_N RB7 output
  MCU_INT_N_SetDigitalMode();
  MCU_INT_N_SetDigitalOutput();
  MCU_INT_N_SetOpenDrain();
  MCU_INT_N_SetHigh();

  // PWR_LED_CTRL RC0 output
  PWR_LED_CTRL_SetDigitalMode();
  PWR_LED_CTRL_SetDigitalOutput();
  PWR_LED_CTRL_SetOpenDrain();
  PWR_LED_CTRL_SetHigh();

  // CHG_DISA RC5 output
  CHG_DISA_SetDigitalMode();
  CHG_DISA_SetDigitalOutput();
  CHG_DISA_SetOpenDrain();
  CHG_DISA_SetHigh();

  //PI_5V_DISA RC4
  PI_5V_DISA_SetDigitalMode();
  PI_5V_DISA_SetLow();
  PI_5V_DISA_SetDigitalOutput();
  PI_5V_DISA_SetPushPull();
  

  // // I2C_SEL_N RC4 output
  // I2C_SEL_N_SetDigitalMode();
  // I2C_SEL_N_SetDigitalOutput();
  // I2C_SEL_N_SetOpenDrain();

  //SCL RB6 i2c clock output
  SCL_SetDigitalMode();
  SCL_SetDigitalOutput();
  SCL_SetLow();
  SCL_SetOpenDrain();
  RB6FEAT = 0x21; //todo make it nicer
  I2C1SCLPPS = 0xE;  //RB6->I2C1:SCL1;
  RB6PPS = 0x1C;  //RB6->I2C1:SCL1;

  //SDA RB5 i2c data 
  SDA_SetDigitalMode();
  SDA_SetDigitalOutput();
  SDA_SetLow();
  SDA_SetOpenDrain();
  RB5FEAT = 0x21; //todo make it nicer
  I2C1SDAPPS = 0xD;  //RB5->I2C1:SDA1;
  RB5PPS = 0x1D;  //RB5->I2C1:SDA1;


  // GPIO_RA5 RA5 output
  GPIO_RA5_SetDigitalMode();
  GPIO_RA5_SetDigitalOutput();
  GPIO_RA5_SetLow();

  //IO_RA0 RA0 output
  IO_RA0_SetDigitalMode();
  IO_RA0_SetDigitalOutput();
  IO_RA0_SetHigh();

  //IO_RA1 RA1 output
  IO_RA1_SetDigitalMode();
  IO_RA1_SetDigitalOutput();
  IO_RA1_SetHigh();
}