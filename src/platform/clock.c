
#include <xc.h>
#include "clock.h"

static void CLOCK_Initialize64MHz(void);
static void CLOCK_Initialize1MHz(void);
void CLOCK_Initialize(void)
{
    #if _XTAL_FREQ == 64000000
        CLOCK_Initialize64MHz();
    #elif _XTAL_FREQ == 1000000
        CLOCK_Initialize1MHz();
    #endif

}
void CLOCK_Initialize1MHz(void)
{
    // Set the CLOCK CONTROL module to the options selected in the user interface.
    //NDIV 1; NOSC HFINTOSC; 
    OSCCON1 = 0x60;
    //SOSCPWR High power; CSWHOLD may proceed; 
    OSCCON3 = 0x40;
    //EXTOEN disabled; HFOEN disabled; MFOEN disabled; LFOEN disabled; SOSCEN disabled; ADOEN disabled; PLLEN disabled; 
    OSCEN = 0x0;
    //HFFRQ 1_MHz; 
    OSCFRQ = 0x0;
    //TUN undefined; 
    OSCTUNE = 0x0;
    //ACTEN disabled; ACTUD enabled; 
    ACTCON = 0x0;
    //FSCMFEV detected; FSCMFFI enabled; FSCMPEV detected; FSCMPFI enabled; FSCMSEV detected; FSCMSFI enabled; 
    FSCMCON = 0x0;

}
void CLOCK_Initialize64MHz(void)
{
    // Set the CLOCK CONTROL module to the options selected in the user interface.
    //NDIV 1; NOSC HFINTOSC; 
    OSCCON1 = 0x60;
    //SOSCPWR High power; CSWHOLD may proceed; 
    OSCCON3 = 0x40;
    //EXTOEN disabled; HFOEN disabled; MFOEN disabled; LFOEN disabled; SOSCEN disabled; ADOEN disabled; PLLEN disabled; 
    OSCEN = 0x0;
    //HFFRQ 64_MHz; 
    OSCFRQ = 0x8;
    //TUN undefined; 
    OSCTUNE = 0x0;
    //ACTEN disabled; ACTUD enabled; 
    ACTCON = 0x0;
    //FSCMFEV detected; FSCMFFI enabled; FSCMPEV detected; FSCMPFI enabled; FSCMSEV detected; FSCMSFI enabled; 
    FSCMCON = 0x0;

}