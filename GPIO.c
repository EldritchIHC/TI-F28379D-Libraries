/*
 * GPIO.c
 *
 *  Created on: 15 feb. 2023
 *      Author: PAul
 */
/* Project Headers */
#include "F28x_Project.h"

/* System Headefiles*/
#include <stdlib.h>

/* Own Headerfiles */
#include "SYSPARAM.h"
#include "GPIO.h"

/* Extern Headerfiles */

/* Function Prototypes */
void GPIO_vConverterPrechargeModeConfig();
void GPIO_vConverterBalanceModeConfig();
void GPIO_vConverterOnModeConfig();
void GPIO_vConverterOffModeConfig();
void GPIO_vConfig();
void GPIO_vStrobeMOS1();
void GPIO_vStrobeMOS2();
void GPIO_vStrobeMOS3();
void GPIO_vStrobeMOS4();
void GPIO_vStrobeMOS1And3();
void GPIO_vStrobeMOS2And4();
/* Global Variables */
/*
On reset, GPIOs are in input mode and have the internal pullups disabled. An un-driven input can float to
a mid-rail voltage and cause wasted shoot-through current on the input buffer. The user should always put
each GPIO in one of these configurations:
• Input mode and driven on the board by another component to a level above Vih or below Vil
• Input mode with GPIO internal pullup enabled
• Output mode
On devices with lesser pin count packages, pull-ups on unbonded GPIOs are by default enabled to
prevent floating inputs. The user should take care to avoid disabling these pullups in their application
code.
On devices in the 176 PTPor 100 PZP packages, the pullups for any internally unbonded GPIO must be
enabled to prevent floating inputs. TI has provided functions in controlSUITE/C2000Ware which users can
call to enable the pullup on any unbonded GPIO for the package they are using. This function,
GPIO_EnabledUnbondedIOPullups(), resides in the (Device)_Sysctrl.c file and is called by default from
InitSysCtrl(). The user should take care to avoid disabling these pullups in their application code.
*/
void GPIO_vConfig()
{
    EALLOW;
    #ifdef DEBUG
    //Time profiling GPIO
    GpioCtrlRegs.GPADIR.bit.GPIO10=1;
    #endif
    //Disabel pull-up
    GpioCtrlRegs.GPAPUD.bit.GPIO6 = 1;// Disable pull-up on GPIO6 (EPWM4A)
    GpioCtrlRegs.GPAPUD.bit.GPIO7 = 1;// Disable pull-up on GPIO7 (EPWM4B)
    GpioCtrlRegs.GPAPUD.bit.GPIO8 = 1;// Disable pull-up on GPIO8 (EPWM5A)
    GpioCtrlRegs.GPAPUD.bit.GPIO9 = 1;// Disable pull-up on GPIO9 (EPWM5B)
    //GpioCtrlRegs.GPAPUD.bit.GPIO10 = 1;// Disable pull-up on GPIO10 (EPWM6A)
    //GpioCtrlRegs.GPAPUD.bit.GPIO11 = 1;// Disable pull-up on GPIO11 (EPWM6B)
    //GPIO MUXing
    GpioCtrlRegs.GPAMUX1.bit.GPIO6  = EPWM_MODE;// Configure GPIO6 as EPWM4A
    GpioCtrlRegs.GPAMUX1.bit.GPIO7  = EPWM_MODE;// Configure GPIO7 as EPWM4B
    GpioCtrlRegs.GPAMUX1.bit.GPIO8  = EPWM_MODE;// Configure GPIO8 as EPWM5A
    GpioCtrlRegs.GPAMUX1.bit.GPIO9  = EPWM_MODE;// Configure GPIO9 as EPWM5B
   // GpioCtrlRegs.GPAMUX1.bit.GPIO10 = EPWM_MODE;// Configure GPIO10 as EPWM6A
    //GpioCtrlRegs.GPAMUX1.bit.GPIO11 = EPWM_MODE;// Configure GPIO11 as EPWM6B

    EDIS;
}


void GPIO_vConverterPrechargeModeConfig()
{
    EALLOW;
    //Force low
    //GpioDataRegs.GPACLEAR.bit.GPIO6 = 1;
    GpioDataRegs.GPACLEAR.bit.GPIO7 = 1;
    //GpioDataRegs.GPACLEAR.bit.GPIO8 = 1;
    GpioDataRegs.GPACLEAR.bit.GPIO9 = 1;
    GpioDataRegs.GPACLEAR.all = 0x00000280;

    //Force high
    //GpioDataRegs.GPASET.bit.GPIO6 = 1;
    //GpioDataRegs.GPASET.bit.GPIO7 = 1;
    //GpioDataRegs.GPASET.bit.GPIO8 = 1;
    //GpioDataRegs.GPASET.bit.GPIO9 = 1;
    GpioDataRegs.GPASET.all = 0x00000140;

    //GPIO Direction
    GpioCtrlRegs.GPADIR.bit.GPIO6 = DIRECTION_OUT;
    GpioCtrlRegs.GPADIR.bit.GPIO7 = DIRECTION_OUT;
    GpioCtrlRegs.GPADIR.bit.GPIO8 = DIRECTION_OUT;
    GpioCtrlRegs.GPADIR.bit.GPIO9 = DIRECTION_OUT;

    //GPIO MUXing
    GpioCtrlRegs.GPAMUX1.bit.GPIO6 = GPIO_MODE;// Configure GPIO6 as GPIO
    GpioCtrlRegs.GPAMUX1.bit.GPIO7 = GPIO_MODE;// Configure GPIO7 as GPIO
    GpioCtrlRegs.GPAMUX1.bit.GPIO8 = GPIO_MODE;// Configure GPIO8 as GPIO
    GpioCtrlRegs.GPAMUX1.bit.GPIO9 = GPIO_MODE;// Configure GPIO9 as GPIO
    EDIS;
}

//PWM4 and PWM5 on, PWM6 forced low
void GPIO_vConverterOnModeConfig()
{
    EALLOW;
    //GPIO MUXing
    GpioCtrlRegs.GPAMUX1.bit.GPIO6 = EPWM_MODE;// Configure GPIO6 as EPWM4A
    GpioCtrlRegs.GPAMUX1.bit.GPIO7 = EPWM_MODE;// Configure GPIO7 as EPWM4B
    GpioCtrlRegs.GPAMUX1.bit.GPIO8 = EPWM_MODE;// Configure GPIO8 as EPWM5A
    GpioCtrlRegs.GPAMUX1.bit.GPIO9 = EPWM_MODE;// Configure GPIO9 as EPWM5B
    EDIS;
}

void GPIO_vConverterOffModeConfig()
{
    EALLOW;
    //Force low
    GpioDataRegs.GPACLEAR.bit.GPIO6 = 1;
    GpioDataRegs.GPACLEAR.bit.GPIO7 = 1;
    GpioDataRegs.GPACLEAR.bit.GPIO8 = 1;
    GpioDataRegs.GPACLEAR.bit.GPIO9 = 1;
    //GPIO Direction
    GpioCtrlRegs.GPADIR.bit.GPIO6 = DIRECTION_OUT;
    GpioCtrlRegs.GPADIR.bit.GPIO7 = DIRECTION_OUT;
    GpioCtrlRegs.GPADIR.bit.GPIO8 = DIRECTION_OUT;
    GpioCtrlRegs.GPADIR.bit.GPIO9 = DIRECTION_OUT;
    //GPIO MUXing
    GpioCtrlRegs.GPAMUX1.bit.GPIO6 = GPIO_MODE;// Configure GPIO6 as GPIO
    GpioCtrlRegs.GPAMUX1.bit.GPIO7 = GPIO_MODE;// Configure GPIO7 as GPIO
    GpioCtrlRegs.GPAMUX1.bit.GPIO8 = GPIO_MODE;// Configure GPIO8 as GPIO
    GpioCtrlRegs.GPAMUX1.bit.GPIO9 = GPIO_MODE;// Configure GPIO9 as GPIO
    EDIS;
}

/*
void GPIO_vStrobeMOS1()
{
    EALLOW;
    //GPIO MUXing
    GpioCtrlRegs.GPAMUX1.bit.GPIO6 = GPIO_MODE;// Configure GPIO6 as EPWM4A
    GpioCtrlRegs.GPAMUX1.bit.GPIO7 = GPIO_MODE;// Configure GPIO7 as GPIO
    GpioCtrlRegs.GPAMUX1.bit.GPIO8 = GPIO_MODE;// Configure GPIO8 as EPWM5A
    GpioCtrlRegs.GPAMUX1.bit.GPIO9 = GPIO_MODE;// Configure GPIO9 as GPIO
    //GPIO Direction
    GpioCtrlRegs.GPADIR.bit.GPIO7 = DIRECTION_OUT;
    GpioCtrlRegs.GPADIR.bit.GPIO6 = DIRECTION_OUT;
    GpioCtrlRegs.GPADIR.bit.GPIO8 = DIRECTION_OUT;
    GpioCtrlRegs.GPADIR.bit.GPIO9 = DIRECTION_OUT;
    //Force high
    GpioDataRegs.GPASET.bit.GPIO9 = 1;;
    EALLOW;
    EALLOW;
    //Force low
    GpioDataRegs.GPACLEAR.bit.GPIO9 = 1;
    EDIS;
}

void GPIO_vStrobeMOS2()
{
    EALLOW;
    //GPIO MUXing
    GpioCtrlRegs.GPAMUX1.bit.GPIO6 = GPIO_MODE;// Configure GPIO6 as EPWM4A
    GpioCtrlRegs.GPAMUX1.bit.GPIO7 = GPIO_MODE;// Configure GPIO7 as GPIO
    GpioCtrlRegs.GPAMUX1.bit.GPIO8 = GPIO_MODE;// Configure GPIO8 as EPWM5A
    GpioCtrlRegs.GPAMUX1.bit.GPIO9 = GPIO_MODE;// Configure GPIO9 as GPIO
    //GPIO Direction
    GpioCtrlRegs.GPADIR.bit.GPIO7 = DIRECTION_OUT;
    GpioCtrlRegs.GPADIR.bit.GPIO6 = DIRECTION_OUT;
    GpioCtrlRegs.GPADIR.bit.GPIO8 = DIRECTION_OUT;
    GpioCtrlRegs.GPADIR.bit.GPIO9 = DIRECTION_OUT;
    //Force high
    GpioDataRegs.GPASET.bit.GPIO8 = 1;;
    EDIS;
    EDIS;
    EALLOW;
    //Force low
    GpioDataRegs.GPACLEAR.bit.GPIO8 = 1;
    EDIS;
}

void GPIO_vStrobeMOS3()
{
    EALLOW;
    //GPIO MUXing
    GpioCtrlRegs.GPAMUX1.bit.GPIO6 = GPIO_MODE;// Configure GPIO6 as EPWM4A
    GpioCtrlRegs.GPAMUX1.bit.GPIO7 = GPIO_MODE;// Configure GPIO7 as GPIO
    GpioCtrlRegs.GPAMUX1.bit.GPIO8 = GPIO_MODE;// Configure GPIO8 as EPWM5A
    GpioCtrlRegs.GPAMUX1.bit.GPIO9 = GPIO_MODE;// Configure GPIO9 as GPIO
    //GPIO Direction
    GpioCtrlRegs.GPADIR.bit.GPIO7 = DIRECTION_OUT;
    GpioCtrlRegs.GPADIR.bit.GPIO6 = DIRECTION_OUT;
    GpioCtrlRegs.GPADIR.bit.GPIO8 = DIRECTION_OUT;
    GpioCtrlRegs.GPADIR.bit.GPIO9 = DIRECTION_OUT;
    //Force high
    GpioDataRegs.GPASET.bit.GPIO7 = 1;
    EALLOW;
    EALLOW;
    //Force low
    GpioDataRegs.GPACLEAR.bit.GPIO7 = 1;
    EDIS;
}

void GPIO_vStrobeMOS4()
{
    EALLOW;
    //GPIO MUXing
    GpioCtrlRegs.GPAMUX1.bit.GPIO6 = GPIO_MODE;// Configure GPIO6 as EPWM4A
    GpioCtrlRegs.GPAMUX1.bit.GPIO7 = GPIO_MODE;// Configure GPIO7 as GPIO
    GpioCtrlRegs.GPAMUX1.bit.GPIO8 = GPIO_MODE;// Configure GPIO8 as EPWM5A
    GpioCtrlRegs.GPAMUX1.bit.GPIO9 = GPIO_MODE;// Configure GPIO9 as GPIO
    //GPIO Direction
    GpioCtrlRegs.GPADIR.bit.GPIO7 = DIRECTION_OUT;
    GpioCtrlRegs.GPADIR.bit.GPIO6 = DIRECTION_OUT;
    GpioCtrlRegs.GPADIR.bit.GPIO8 = DIRECTION_OUT;
    GpioCtrlRegs.GPADIR.bit.GPIO9 = DIRECTION_OUT;
    //Force high
    GpioDataRegs.GPASET.bit.GPIO6 = 1;
    EALLOW;
    EALLOW;
    //Force low
    GpioDataRegs.GPACLEAR.bit.GPIO6 = 1;
    EDIS;
}

void GPIO_vStrobeMOS1And3()
{
    EALLOW;
    //GPIO MUXing
    GpioCtrlRegs.GPAMUX1.bit.GPIO6 = GPIO_MODE;// Configure GPIO6 as EPWM4A
    GpioCtrlRegs.GPAMUX1.bit.GPIO7 = GPIO_MODE;// Configure GPIO7 as GPIO
    GpioCtrlRegs.GPAMUX1.bit.GPIO8 = GPIO_MODE;// Configure GPIO8 as EPWM5A
    GpioCtrlRegs.GPAMUX1.bit.GPIO9 = GPIO_MODE;// Configure GPIO9 as GPIO
    //GPIO Direction
    GpioCtrlRegs.GPADIR.bit.GPIO7 = DIRECTION_OUT;
    GpioCtrlRegs.GPADIR.bit.GPIO6 = DIRECTION_OUT;
    GpioCtrlRegs.GPADIR.bit.GPIO8 = DIRECTION_OUT;
    GpioCtrlRegs.GPADIR.bit.GPIO9 = DIRECTION_OUT;
    //Force high
    //GpioDataRegs.GPASET.bit.GPIO9 = 1;
    //GpioDataRegs.GPASET.bit.GPIO7 = 1;
    GpioDataRegs.GPASET.all = 0x00000280;
    EALLOW;
    EALLOW;
    //Force low
    //GpioDataRegs.GPACLEAR.bit.GPIO9 = 1;
    //GpioDataRegs.GPACLEAR.bit.GPIO7 = 1;
    GpioDataRegs.GPACLEAR.all = 0x00000280;
    EDIS;
}

void GPIO_vStrobeMOS2And4()
{
    EALLOW;
    //GPIO MUXing
    GpioCtrlRegs.GPAMUX1.bit.GPIO6 = GPIO_MODE;// Configure GPIO6 as EPWM4A
    GpioCtrlRegs.GPAMUX1.bit.GPIO7 = GPIO_MODE;// Configure GPIO7 as GPIO
    GpioCtrlRegs.GPAMUX1.bit.GPIO8 = GPIO_MODE;// Configure GPIO8 as EPWM5A
    GpioCtrlRegs.GPAMUX1.bit.GPIO9 = GPIO_MODE;// Configure GPIO9 as GPIO
    //GPIO Direction
    GpioCtrlRegs.GPADIR.bit.GPIO7 = DIRECTION_OUT;
    GpioCtrlRegs.GPADIR.bit.GPIO6 = DIRECTION_OUT;
    GpioCtrlRegs.GPADIR.bit.GPIO8 = DIRECTION_OUT;
    GpioCtrlRegs.GPADIR.bit.GPIO9 = DIRECTION_OUT;
    //Force high
    //GpioDataRegs.GPASET.bit.GPIO8 = 1;
    //GpioDataRegs.GPASET.bit.GPIO6 = 1;
    GpioDataRegs.GPASET.all = 0x00000140;
    EALLOW;
    EALLOW;
    //Force low
    //GpioDataRegs.GPACLEAR.bit.GPIO8 = 1;
    //GpioDataRegs.GPACLEAR.bit.GPIO6 = 1;
    GpioDataRegs.GPACLEAR.all = 0x00000140;
    EDIS;
}
*/
