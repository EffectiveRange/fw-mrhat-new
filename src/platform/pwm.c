#include "system.h"
#include "pwm.h"
#include "clock.h"
typedef void (*void_cb_t)(void);

void_cb_t PWM1_16BIT_Slice1Output1_InterruptHandler=NULL;
void_cb_t PWM1_16BIT_Slice1Output2_InterruptHandler=NULL;
void_cb_t PWM1_16BIT_Period_InterruptHandler=NULL;
#if _XTAL_FREQ == 64000000
    #define PWM_CLOCK 64000 // 64KHz
#elif _XTAL_FREQ == 1000000
    #define PWM_CLOCK 1000 // 1KHz
#endif
void PWM_DutyCycleSetPercentage_Slice1(uint16_t duty_cycle)
{
    uint16_t PWM_DytyCyclePercentage = (uint16_t) ((1.0f - ((double) duty_cycle / (double) 100.)) * (PWM_CLOCK-1));
    PWM1_16BIT_SetSlice1Output1DutyCycleRegister(PWM_DytyCyclePercentage);
    PWM1_16BIT_LoadBufferRegisters();
}
void PWM1_16BIT_Initialize(void)
{
    //pwm pin = RC0 PWR_LED_CTRL
    TRISCbits.TRISC0 = 0; //out
    LATCbits.LATC0 = 0; //start high
    // ANSELCbits.ANSELC0 = 0; //digital
    RC0FEAT = 0x20;
    RC0PPS = 0x07;  //RC0->PWM1_16BIT:PWM11;


    //init pwm regs
    #if _XTAL_FREQ == 64000000
        //PWMERS External Reset Disabled; 
        PWM1ERS = 0x0;

        //PWMCLK FOSC; 
        PWM1CLK = 0x2;

        //PWMLDS Autoload disabled; 
        PWM1LDS = 0x0;

        //PWMPRL 255; 
        PWM1PRL = 0xFF;

        //PWMPRH 249; 
        PWM1PRH = 0xF9;

        //PWMCPRE No prescale; 
        PWM1CPRE = 0x0;

        //PWMPIPOS No postscale; 
        PWM1PIPOS = 0x0;

        //PWMS1P1IF PWM1 output match did not occur; PWMS1P2IF PWM2 output match did not occur; 
        PWM1GIR = 0x0;

        //PWMS1P1IE disabled; PWMS1P2IE disabled; 
        PWM1GIE = 0x0;

        //PWMPOL1 disabled; PWMPOL2 disabled; PWMPPEN disabled; PWMMODE Left aligned mode; 
        PWM1S1CFG = 0x0;

        //PWMS1P1L 0; 
        PWM1S1P1L = 0x0;

        //PWMS1P1H 250; 
        PWM1S1P1H = 0xFA;

        //PWMS1P2L 128; 
        PWM1S1P2L = 0x80;

        //PWMS1P2H 2; 
        PWM1S1P2H = 0x2;
    #elif _XTAL_FREQ == 1000000
        //PWMERS External Reset Disabled; 
        PWM1ERS = 0x0;

        //PWMCLK FOSC; 
        PWM1CLK = 0x2;

        //PWMLDS Autoload disabled; 
        PWM1LDS = 0x0;

        //PWMPRL 231; 
        PWM1PRL = 0xE7;

        //PWMPRH 3; 
        PWM1PRH = 0x3;

        //PWMCPRE No prescale; 
        PWM1CPRE = 0x0;

        //PWMPIPOS No postscale; 
        PWM1PIPOS = 0x0;

        //PWMS1P1IF PWM1 output match did not occur; PWMS1P2IF PWM2 output match did not occur; 
        PWM1GIR = 0x0;

        //PWMS1P1IE disabled; PWMS1P2IE disabled; 
        PWM1GIE = 0x0;

        //PWMPOL1 disabled; PWMPOL2 disabled; PWMPPEN disabled; PWMMODE Left aligned mode; 
        PWM1S1CFG = 0x0;

        //PWMS1P1L 232; 
        PWM1S1P1L = 0xE8;

        //PWMS1P1H 3; 
        PWM1S1P1H = 0x3;

        //PWMS1P2L 10; 
        PWM1S1P2L = 0xA;

        //PWMS1P2H 0; 
        PWM1S1P2H = 0x0;
    #endif
    //Clear PWM1_16BIT period interrupt flag
    PIR4bits.PWM1PIF = 0;
    
    //Clear PWM1_16BIT interrupt flag
    PIR4bits.PWM1IF = 0;
    
    //Clear PWM1_16BIT slice 1, output 1 interrupt flag
    PWM1GIRbits.S1P1IF = 0;
    
    //Clear PWM1_16BIT slice 1, output 2 interrupt flag
    PWM1GIRbits.S1P2IF = 0;
    
    //PWM1_16BIT interrupt enable bit
    PIE4bits.PWM1IE = 0;
    
    //PWM1_16BIT period interrupt enable bit
    PIE4bits.PWM1PIE = 1;
    
    //PWMEN enabled; PWMLD disabled; PWMERSPOL disabled; PWMERSNOW disabled; 
    PWM1CON = 0x80;
}

void PWM1_16BIT_Enable(void)
{
    PWM1CON |= _PWM1CON_EN_MASK;
}

void PWM1_16BIT_Disable(void)
{
    PWM1CON &= (~_PWM1CON_EN_MASK);
}

void PWM1_16BIT_WritePeriodRegister(uint16_t periodCount)
{
    PWM1PRL = (uint8_t)periodCount;
    PWM1PRH= (uint8_t)(periodCount >> 8);
}

void PWM1_16BIT_SetSlice1Output1DutyCycleRegister(uint16_t registerValue)
{    
    PWM1S1P1L = (uint8_t)(registerValue);
    PWM1S1P1H = (uint8_t)(registerValue >> 8);
}

void PWM1_16BIT_SetSlice1Output2DutyCycleRegister(uint16_t registerValue)
{        
    PWM1S1P2L = (uint8_t)(registerValue);
    PWM1S1P2H = (uint8_t)(registerValue >> 8);
}

void PWM1_16BIT_LoadBufferRegisters(void)
{
    //Load the period and duty cycle registers on the next period event
    PWM1CONbits.LD = 1;
}

void __interrupt(irq(PWM1),base(8)) PWM1_16BIT_PWMI_ISR()
{
    PIR4bits.PWM1IF = 0;
    if((PWM1GIEbits.S1P1IE == 1) && (PWM1GIRbits.S1P1IF == 1))
    {
        PWM1GIRbits.S1P1IF = 0;
        if(PWM1_16BIT_Slice1Output1_InterruptHandler != NULL)
        {
            PWM1_16BIT_Slice1Output1_InterruptHandler();
        }
    }
    else if((PWM1GIEbits.S1P2IE == 1) && (PWM1GIRbits.S1P2IF == 1))
    {
        PWM1GIRbits.S1P2IF = 0;
        if(PWM1_16BIT_Slice1Output2_InterruptHandler != NULL)
        {
            PWM1_16BIT_Slice1Output2_InterruptHandler();
        }
    }
    else
    {
        // No Action 
    }
}

void __interrupt(irq(PWM1P),base(8)) PWM1_16BIT_PWMPI_ISR()
{
    PIR4bits.PWM1PIF = 0;
    if(PWM1_16BIT_Period_InterruptHandler != NULL)
    {
        PWM1_16BIT_Period_InterruptHandler();
    }
}
void PWM1_16BIT_Slice1Output1_SetInterruptHandler(void (* InterruptHandler)(void))
{
    PWM1_16BIT_Slice1Output1_InterruptHandler = InterruptHandler;
}

void PWM1_16BIT_Slice1Output2_SetInterruptHandler(void (* InterruptHandler)(void))
{
    PWM1_16BIT_Slice1Output2_InterruptHandler = InterruptHandler;
}

void PWM1_16BIT_Period_SetInterruptHandler(void (* InterruptHandler)(void))
{
    PWM1_16BIT_Period_InterruptHandler = InterruptHandler;
}
