
#include "system.h"

#include "plat_gpio.h"
void Plat_GPIO_Interrupt_Init() {
  // rtc
  RTC_IRQ_N_EnableIOCNegativeEdge();
  // bq int
  BQ_INT_EnableIOCNegativeEdge();
  // pirun
  PI_RUN_EnableIOCNegativeEdge();

  //enable peripheriel interrupe enable : IOC(interrupt on change) interrupt enable
  PIE3bits.IOCIE = 1;
}

typedef void (*void_cb_t)(void);

void_cb_t RTC_IRQ_N_Callback = NULL;
void_cb_t BQ_INT_Callback = NULL;
void_cb_t PI_RUN_Callback = NULL;
void Plat_GPIO_Register_RTC_IRQ_N_Callback(void (*callback)(void)) {
  RTC_IRQ_N_Callback = callback;
}
void Plat_GPIO_Register_BQ_INT_Callback(void (*callback)(void)) {
  BQ_INT_Callback = callback;
}
void Plat_GPIO_Register_PI_RUN_Callback(void (*callback)(void)) {
  PI_RUN_Callback = callback;
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
  // interrupt on change for pin BQ_INT_N
  if (IOCCFbits.IOCCF7 == 1) {
    if (BQ_INT_Callback != NULL) {
      BQ_INT_Callback();
    }
    IOCCFbits.IOCCF7 = 0;
  }
}

void Plat_GPIO_Init(void) {
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

  // BQ_INT_N open drain input
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

  // I2C_SEL_N RC4 output
  I2C_SEL_N_SetDigitalMode();
  I2C_SEL_N_SetDigitalOutput();
  I2C_SEL_N_SetOpenDrain();
}