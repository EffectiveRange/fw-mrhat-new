
#include "system.h"

void PWM_DutyCycleSetPercentage_Slice1(uint16_t duty_cycle);
void PWM1_16BIT_Initialize(void);
void PWM1_16BIT_Enable(void);
void PWM1_16BIT_Disable(void);
void PWM1_16BIT_WritePeriodRegister(uint16_t periodCount);

void PWM1_16BIT_SetSlice1Output1DutyCycleRegister(uint16_t registerValue);

void PWM1_16BIT_SetSlice1Output2DutyCycleRegister(uint16_t registerValue);
void PWM1_16BIT_LoadBufferRegisters(void);

void PWM1_16BIT_Slice1Output1_SetInterruptHandler(void (* InterruptHandler)(void));

void PWM1_16BIT_Slice1Output2_SetInterruptHandler(void (* InterruptHandler)(void));

void PWM1_16BIT_Period_SetInterruptHandler(void (* InterruptHandler)(void));
