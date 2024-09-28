

#include "interrupt.h"
#include "system.h"
#include <stdbool.h>
#include "plat_gpio.h"
void (*INT0_InterruptHandler)(void);
void (*INT1_InterruptHandler)(void);
void (*INT2_InterruptHandler)(void);

void  INTERRUPT_Initialize (void)
{
    INTCON0bits.IPEN = 1;

    bool state = (unsigned char)GIE;
    GIE = 0;
    IVTLOCK = 0x55;
    IVTLOCK = 0xAA;
    IVTLOCKbits.IVTLOCKED = 0x00; // unlock IVT

    IVTBASEU = 0;
    IVTBASEH = 0;
    IVTBASEL = 8;

    IVTLOCK = 0x55;
    IVTLOCK = 0xAA;
    IVTLOCKbits.IVTLOCKED = 0x01; // lock IVT

    GIE = state;
    // Assign peripheral interrupt priority vectors
    IPR0bits.INT0IP = 1;
    IPR0bits.INT1IP = 1;
    IPR0bits.INT2IP = 1;
    IPR3bits.IOCIP = 1;
    IPR3bits.TMR0IP = 1;
    IPR3bits.TMR1IP = 1;
    IPR4bits.PWM1PIP = 1;
    IPR7bits.I2C1RXIP = 1;
    IPR7bits.I2C1TXIP = 1;
    IPR7bits.I2C1IP = 1;
    IPR7bits.I2C1EIP = 1;

    // Clear the interrupt flag
    // Set the external interrupt edge detect
    EXT_INT0_InterruptFlagClear();   
    EXT_INT0_fallingEdgeSet();    
    // Set Default Interrupt Handler
    INT0_SetInterruptHandler(INT0_DefaultInterruptHandler);
    EXT_INT0_InterruptEnable();

    // Clear the interrupt flag
    // Set the external interrupt edge detect
    EXT_INT1_InterruptFlagClear();   
    EXT_INT1_fallingEdgeSet();    
    // Set Default Interrupt Handler
    INT1_SetInterruptHandler(INT1_DefaultInterruptHandler);
    EXT_INT1_InterruptEnable();

    // Clear the interrupt flag
    // Set the external interrupt edge detect
    EXT_INT2_InterruptFlagClear();   
    EXT_INT2_fallingEdgeSet();    
    // Set Default Interrupt Handler
    INT2_SetInterruptHandler(INT2_DefaultInterruptHandler);
    EXT_INT2_InterruptEnable();

}

void __interrupt(irq(default),base(8)) Default_ISR()
{
}

void __interrupt(irq(IOC), base(8)) IOC_ISR()
{
    PIN_MANAGER_IOC();
}

/**
 * @ingroup interrupt
 * @brief Executes whenever the signal on the INT0 pin transitions on the selected edge.
 * @pre INTERRUPT_Initialize() is already called.
 * @param None.
 * @return None.
 */
void __interrupt(irq(INT0),base(8)) INT0_ISR()
{
    EXT_INT0_InterruptFlagClear();

    // Callback function gets called everytime this ISR executes
    INT0_CallBack();    
}


void INT0_CallBack(void)
{
    // Add your custom callback code here
    if(INT0_InterruptHandler)
    {
        INT0_InterruptHandler();
    }
}

void INT0_SetInterruptHandler(void (* InterruptHandler)(void)){
    INT0_InterruptHandler = InterruptHandler;
}

void INT0_DefaultInterruptHandler(void){
    // add your INT0 interrupt custom code
    // or set custom function using INT0_SetInterruptHandler()
}

/**
 * @ingroup interrupt
 * @brief Executes whenever the signal on the INT1 pin transitions on the selected edge.
 * @pre INTERRUPT_Initialize() is already called.
 * @param None.
 * @return None.
 */
void __interrupt(irq(INT1),base(8)) INT1_ISR()
{
    EXT_INT1_InterruptFlagClear();

    // Callback function gets called everytime this ISR executes
    INT1_CallBack();    
}


void INT1_CallBack(void)
{
    // Add your custom callback code here
    if(INT1_InterruptHandler)
    {
        INT1_InterruptHandler();
    }
}

void INT1_SetInterruptHandler(void (* InterruptHandler)(void)){
    INT1_InterruptHandler = InterruptHandler;
}

void INT1_DefaultInterruptHandler(void){
    // add your INT1 interrupt custom code
    // or set custom function using INT1_SetInterruptHandler()
}

/**
 * @ingroup interrupt
 * @brief Executes whenever the signal on the INT2 pin transitions on the selected edge.
 * @pre INTERRUPT_Initialize() is already called.
 * @param None.
 * @return None.
 */
void __interrupt(irq(INT2),base(8)) INT2_ISR()
{
    EXT_INT2_InterruptFlagClear();

    // Callback function gets called everytime this ISR executes
    INT2_CallBack();    
}


void INT2_CallBack(void)
{
    // Add your custom callback code here
    if(INT2_InterruptHandler)
    {
        INT2_InterruptHandler();
    }
}

void INT2_SetInterruptHandler(void (* InterruptHandler)(void)){
    INT2_InterruptHandler = InterruptHandler;
}

void INT2_DefaultInterruptHandler(void){
    // add your INT2 interrupt custom code
    // or set custom function using INT2_SetInterruptHandler()
}

/**
 End of File
*/
