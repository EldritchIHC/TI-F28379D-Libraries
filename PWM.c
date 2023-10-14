/*
 * PWM.c
 *
 *  Created on: 23 aug. 2022
 *      Author: Paul
 */
/* Project Headers */
#include "F28x_Project.h"

/* System Headefiles*/
#include <stdlib.h>

/* Own Headerfiles */
#include "SYSPARAM.h"
#include "PWM.h"

#include "CLA_SHARED.h"

/* Extern Headerfiles */

/* Function Prototypes */

void PWM_vEPWMModuleInit();

void PWM_vEPWM4Init();
void PWM_vEPWM4Config(tstEPWMConfig *PWM);
//void PWM_vEPWM4BuckChargeMode();
void PWM_vEPWM4ConverterMode();
void PWM_vEPWM4DutyUpdate(tstEPWMConfig *PWM, Uint16 u16newDuty);
void PWM_vEPWM4DeadbandUpdate(tstEPWMConfig *PWM);
void PWM_vEPWM4Start();
void PWM_vEPWM4Stop();
void PWM_vEPWM4TripZoneStop();
void PWM_vEPWM4TripZoneClear();
void PWM_vEPWM4SOCEnable();
void PWM_EPWM4GlobalLoad();

void PWM_vEPWM5Init();
void PWM_vEPWM5Config(tstEPWMConfig *PWM);
//void PWM_vEPWM5BuckChargeMode();
void PWM_vEPWM5ConverterMode();
void PWM_vEPWM5DutyUpdate(tstEPWMConfig *PWM, Uint16 u16newDuty);
void PWM_vEPWM5DeadbandUpdate(tstEPWMConfig *PWM);
void PWM_vEPWM5Start();
void PWM_vEPWM5Stop();
void PWM_vEPWM5TripZoneStop();
void PWM_vEPWM5TripZoneClear();
void PWM_vEPWM5SOCEnable();

void PWM_vEPWM_Global_Start();
void PWM_vEPWM_Global_Stop();

interrupt void epwm4_isr(void);
interrupt void epwm5_isr(void);
/* Global Variables */
tstEPWMConfig stPWM1 = PWM1_INIT;
tstEPWMConfig stPWM2 = PWM2_INIT;

//Start Clock signal for all PWM modules
void PWM_vEPWM_Global_Start()
{
    EALLOW;
    CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = ENABLE; //start
    EDIS;
}

//Stop Clock signal for all PWM modules
void PWM_vEPWM_Global_Stop()
{
    EALLOW;
    CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = DISABLE; //start
    EDIS;
}

//Initialize PWM4
//Begin
void PWM_vEPWM4Init()
{
    EALLOW;//scriere registre
    CpuSysRegs.PCLKCR2.bit.EPWM4=1;//Enable PWM4
    CpuSysRegs.PCLKCR0.bit.TBCLKSYNC =DISABLE; // stop
    //CLK Division
    EPwm4Regs.TBCTL.bit.HSPCLKDIV=TB_DIV1;//Divide by 4
    EPwm4Regs.TBCTL.bit.CLKDIV=TB_DIV1;//Divide by 1
    //Sync Output Select
    EPwm4Regs.TBCTL.bit.SYNCOSEL = TB_CTR_ZERO; // Sync Output on CRT = zero;
    //Time Base Counter Register
    EPwm4Regs.TBCTR = 0; // Clear Counter
    //PWM Generation
    //Action qualifier setup
    EPwm4Regs.TBCTL.bit.CTRMODE = TB_FREEZE ;
    #ifdef BUCK
    // A set high on 0, reset low on event
    EPwm4Regs.AQCTLA.bit.ZRO = AQ_CLEAR;          // Set PWM A on ZERO
    EPwm4Regs.AQCTLA.bit.CAU = AQ_SET;        // Clear PWM A on CMPA UP
    //B reset low on 0, set high on event
    EPwm4Regs.AQCTLB.bit.ZRO = AQ_CLEAR;        // Clear PWM B on ZERO
    EPwm4Regs.AQCTLB.bit.CAU = AQ_SET ;          // Set PWM B on CMPB UP
    #endif
    #ifdef BOOST
    // A set high on 0, reset low on event
    EPwm4Regs.AQCTLA.bit.ZRO = AQ_SET;          // Set PWM A on ZERO
    EPwm4Regs.AQCTLA.bit.CAU = AQ_CLEAR;        // Clear PWM A on CMPA UP
    //B reset low on 0, set high on event
    EPwm4Regs.AQCTLB.bit.ZRO = AQ_SET;        // Clear PWM B on ZERO
    EPwm4Regs.AQCTLB.bit.CAU = AQ_CLEAR ;          // Set PWM B on CMPB UP
    #endif
    //Shadow register
    EPwm4Regs.CMPCTL.bit.SHDWAMODE=0;
    EPwm4Regs.CMPCTL.bit.LOADAMODE=0;
    EPwm4Regs.CMPCTL.bit.SHDWBMODE=0;
    EPwm4Regs.CMPCTL.bit.LOADBMODE=0;
    //Deadband Setup
    EPwm4Regs.DBCTL.bit.HALFCYCLE=1;//Double TBCLK
    EPwm4Regs.DBCTL.bit.IN_MODE=2;
    EPwm4Regs.DBCTL.bit.DEDB_MODE=0;
    EPwm4Regs.DBCTL.bit.POLSEL=2;
    EPwm4Regs.DBCTL.bit.OUT_MODE=3;
    EPwm4Regs.DBCTL.bit.OUTSWAP=0;
    EPwm4Regs.DBCTL.bit.LOADFEDMODE=0;
    EPwm4Regs.DBCTL.bit.LOADREDMODE=0;
    //Tripzone
    //InputXbarRegs.INPUT1SELECT=5;//Select GPIO5
    EPwm4Regs.TZSEL.bit.OSHT1=ENABLE;
    EPwm4Regs.TZCTL.bit.TZA = TZ_FORCE_LO;
    EPwm4Regs.TZCTL.bit.TZB = TZ_FORCE_LO;
    //EPWM Interrupt
    EPwm4Regs.ETSEL.bit.INTEN=1;//PWM Interrupt
    EPwm4Regs.ETSEL.bit.INTSEL=1;//Counter equals zero
    EPwm4Regs.ETPS.bit.INTCNT=1;
    EPwm4Regs.ETPS.bit.INTPRD=1;//Interrupt on first event
    //EPWM4 Global load

    EPwm4Regs.GLDCTL.bit.GLDPRD = 1;//One event to generate strobe
    EPwm4Regs.GLDCTL.bit.OSHTMODE =1;
    EPwm4Regs.GLDCTL.bit.GLDMODE = 0;//Load on zero
    EPwm4Regs.GLDCTL.bit.GLD = 1;//All shadow registers use the same reload pulse
    EPwm4Regs.GLDCFG.bit.CMPA_CMPAHR = 1;
    EPwm4Regs.GLDCFG.bit.CMPB_CMPBHR = 1;

    //Interrupt vector
    PieVectTable.EPWM4_INT=&epwm4_isr;
    EDIS;
}
//End
void PWM_vEPWMModuleInit()
{
    EALLOW;
    PieCtrlRegs.PIEIER3.bit.INTx4 = 1;//EPWM4INT
    //PWM4 PWM5
    PWM_vEPWM_Global_Stop();
    PWM_vEPWM4Init();
    PWM_vEPWM5Init();
    PWM_vEPWM4Config(&stPWM1);
    PWM_vEPWM5Config(&stPWM2);
    PWM_vEPWM4Start();
    PWM_vEPWM5Start();
    //Change duty
    PWM_vEPWM4DutyUpdate(&stPWM1, DUTYMIN);
    PWM_vEPWM5DutyUpdate(&stPWM2, DUTYMIN);
    //Config tripzone
    PWM_vEPWM4ConverterMode();
    PWM_vEPWM5ConverterMode();
    //PWM_vEPWM6ConverterMode();
    //Disable PWM
    PWM_vEPWM4TripZoneStop();
    PWM_vEPWM5TripZoneStop();
    //Global load the duty
    PWM_EPWM4GlobalLoad();
    //Enable the clock signal
    PWM_vEPWM_Global_Start();
    //SOC for ADCs A and B
    PWM_vEPWM4SOCEnable();
}
/*PWM4 Functions*/


//Config PWM4
void PWM_vEPWM4Config(tstEPWMConfig *PWM)
{
    Uint32 u32TBPRDon,u32TBPRDoff;
    EALLOW;
    //Period Calculation
    PWM->u32TBPRD=(Uint32)( ( EPWM_CLK_HZ / PWM->u32Freq_hz ) - 1 );//Period Calculation
    u32TBPRDon= ( PWM->u32Duty_percent % 100 ) * PWM->u32TBPRD / 100 ;//On Period
    u32TBPRDoff=( 100 - ( PWM->u32Duty_percent % 100 ) )  * PWM->u32TBPRD / 100;//Off Period
    EPwm4Regs.TBPRD = PWM->u32TBPRD; // EPWM Period
    //Action qualifier setup
    EPwm4Regs.CMPA.bit.CMPA = u32TBPRDon;    // Set compare A value
    EPwm4Regs.CMPB.bit.CMPB = u32TBPRDoff; // Set compare B value
    //Deadband
    //RED FED Calculation
    Uint16 u16DeadBandRED = PWM->u16DeadBandRED_ns / SYS_PERIOD_NS;
    Uint16 u16DeadBandFED = PWM->u16DeadBandFED_ns / SYS_PERIOD_NS;
    EPwm4Regs.DBRED.bit.DBRED = u16DeadBandRED;
    EPwm4Regs.DBFED.bit.DBFED = u16DeadBandFED;
    EDIS;
}

//Start PWM4
void PWM_vEPWM4Start()
{
    EALLOW;
    EPwm4Regs.TBCTL.bit.CTRMODE = TB_COUNT_UP;
    EDIS;
}
//Stop PWM4
void PWM_vEPWM4Stop()
{
    EALLOW;
    EPwm4Regs.TBCTL.bit.CTRMODE = TB_FREEZE;
    EDIS;
}
//Forced Stop
void PWM_vEPWM4TripZoneStop()
{
    EALLOW;
    EPwm4Regs.TZFRC.bit.OST=1;
    EDIS;
}
void PWM_vEPWM4TripZoneClear()
{
    EALLOW;
    EPwm4Regs.TZCLR.bit.OST=1;//Clear OST Flag
    EDIS;
}


//Configure Tripzone for Charge Mode
void PWM_vEPWM4ConverterMode()
{
    EALLOW;
    //Tripzone
    //InputXbarRegs.INPUT1SELECT=4;//Select GPIO4
    EPwm4Regs.TZCLR.bit.OST=1;//Clear OST Flag
    EPwm4Regs.TZSEL.bit.OSHT1=ENABLE;
    EPwm4Regs.TZCTL.bit.TZA = TZ_FORCE_LO;
    EPwm4Regs.TZCTL.bit.TZB = TZ_FORCE_LO;
    EDIS;
}

//Recalculate TBPRD based on new duty value
void PWM_vEPWM4DutyUpdate(tstEPWMConfig *PWM, Uint16 u16newDuty)
{
    Uint32 u32TBPRDon,u32TBPRDoff;
    EALLOW;
    //Update the structure
    PWM->u32Duty_percent=u16newDuty;
    //Recalculate comparator values
    //u32TBPRDon=( PWM->u32Duty_percent % 100 ) * PWM->u32TBPRD / 100 ;//On period
    //u32TBPRDoff=( 100- ( PWM->u32Duty_percent % 100 ) )  * PWM->u32TBPRD / 100;//Off period
    u32TBPRDon=( PWM->u32Duty_percent  ) * PWM->u32TBPRD / 100 ;//On period
    u32TBPRDoff=( 100 - ( PWM->u32Duty_percent  ) )  * PWM->u32TBPRD / 100;//Off period
    //Update comparator values
    EPwm4Regs.CMPA.bit.CMPA =u32TBPRDon;    // Set compare A value
    EPwm4Regs.CMPB.bit.CMPB =u32TBPRDoff; // Set compare B value
    EDIS;
}

void PWM_vEPWM4DeadbandUpdate(tstEPWMConfig *PWM)
{
    EALLOW;
    Uint16 u16DeadBandRED = PWM->u16DeadBandRED_ns / SYS_PERIOD_NS;
    Uint16 u16DeadBandFED = PWM->u16DeadBandFED_ns / SYS_PERIOD_NS;
    EPwm4Regs.DBRED.bit.DBRED = u16DeadBandRED;
    EPwm4Regs.DBFED.bit.DBFED = u16DeadBandFED;
    EDIS;
}

void PWM_vEPWM4SOCEnable()
{
    EALLOW;
    EPwm4Regs.ETSEL.bit.SOCAEN  = ENABLE;    // Enable SOC
    EPwm4Regs.ETSEL.bit.SOCASEL = SOCZERO;   // Event Select
    EPwm4Regs.ETPS.bit.SOCAPRD = 1;//0-no pulse,1-first pulse,2-2nd pulse
    EDIS;
}

void PWM_EPWM4GlobalLoad()
{
    EALLOW;
    EPwm4Regs.GLDCTL2.bit.OSHTLD = 1;
    EDIS;
}
//EPWM4 Interrupt
interrupt void epwm4_isr(void)
{
    EALLOW;
    #ifdef DEBUG
    GpioDataRegs.GPASET.bit.GPIO10=1;
    #endif
    EPwm4Regs.ETCLR.bit.INT = 1;
    //if( CLA_u16Task1IsRunning() == 0 )PWM_vEPWM4DutyUpdate(&stPWM1, (Uint16)fNewDuty );
   // DAC_vDACAWriteValue((Uint16)fNewDuty);
    //+PWM5 duty update
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;
    #ifdef DEBUG
    GpioDataRegs.GPACLEAR.bit.GPIO10=1;
    #endif
    EDIS;
}



/*PWM% Functions*/
void PWM_vEPWM5Init()
{
    EALLOW;
    CpuSysRegs.PCLKCR2.bit.EPWM5=1;//enable pwm1
    CpuSysRegs.PCLKCR0.bit.TBCLKSYNC =0; // stop
    //CLK Division
    EPwm5Regs.TBCTL.bit.HSPCLKDIV=TB_DIV1;//Divide by 4
    EPwm5Regs.TBCTL.bit.CLKDIV=TB_DIV1;//Divide by 1

    //Sync Output Select
    EPwm5Regs.TBCTL.bit.SYNCOSEL = TB_CTR_ZERO; // Sync Output on CRT = zero;
    //Time Base Counter Register
    EPwm5Regs.TBCTR = 0; // Clear Counter
    //PWM Generation
    //Action qualifier setup
    EPwm5Regs.TBCTL.bit.CTRMODE = TB_FREEZE;
    #ifdef BUCK
    // A set high on 0, reset low on event
    EPwm5Regs.AQCTLA.bit.ZRO = AQ_CLEAR;          // Set PWM A on ZERO
    EPwm5Regs.AQCTLA.bit.CAU = AQ_SET;        // Clear PWM A on CMPA UP
    //B reset low on 0, set high on event
    EPwm5Regs.AQCTLB.bit.ZRO = AQ_CLEAR;        // Clear PWM B on ZERO
    EPwm5Regs.AQCTLB.bit.CAU = AQ_SET;          // Set PWM B on CMPB UP
    #endif
    #ifdef BOOST
    // A set high on 0, reset low on event
    EPwm5Regs.AQCTLA.bit.ZRO = AQ_SET;          // Set PWM A on ZERO
    EPwm5Regs.AQCTLA.bit.CAU = AQ_CLEAR;        // Clear PWM A on CMPA UP
    //B reset low on 0, set high on event
    EPwm5Regs.AQCTLB.bit.ZRO = AQ_SET;        // Clear PWM B on ZERO
    EPwm5Regs.AQCTLB.bit.CAU = AQ_CLEAR ;          // Set PWM B on CMPB UP
    #endif
    //Shadow register
    EPwm5Regs.CMPCTL.bit.SHDWAMODE=0;
    EPwm5Regs.CMPCTL.bit.LOADAMODE=0;
    EPwm5Regs.CMPCTL.bit.SHDWBMODE=0;
    EPwm5Regs.CMPCTL.bit.LOADBMODE=0;
    //Deadband
    EPwm5Regs.DBCTL.bit.HALFCYCLE=1;//Double TBCLK
    EPwm5Regs.DBCTL.bit.IN_MODE=2;
    EPwm5Regs.DBCTL.bit.DEDB_MODE=0;
    EPwm5Regs.DBCTL.bit.POLSEL=2;
    EPwm5Regs.DBCTL.bit.OUT_MODE=3;
    EPwm5Regs.DBCTL.bit.OUTSWAP=0;
    EPwm5Regs.DBCTL.bit.LOADFEDMODE=0;
    EPwm5Regs.DBCTL.bit.LOADREDMODE=0;
    //Tripzone
    //InputXbarRegs.INPUT1SELECT=5;//Select GPIO5
    EPwm5Regs.TZSEL.bit.OSHT1=ENABLE;
    EPwm5Regs.TZCTL.bit.TZA = TZ_FORCE_LO;
    EPwm5Regs.TZCTL.bit.TZB = TZ_FORCE_LO;
    //EPWM5 Global load

    EPwm5Regs.GLDCTL.bit.GLDPRD = 1;//One event to generate strobe
    EPwm5Regs.GLDCTL.bit.OSHTMODE =1;
    EPwm5Regs.GLDCTL.bit.GLDMODE = 0;//Load on zero
    EPwm5Regs.GLDCTL.bit.GLD = 1;//All shadow registers use the same reload pulse
    EPwm5Regs.GLDCFG.bit.CMPA_CMPAHR = 1;
    EPwm5Regs.GLDCFG.bit.CMPB_CMPBHR = 1;
    EPwm5Regs.EPWMXLINK.bit.GLDCTL2LINK = 3;
    EPwm5Regs.EPWMXLINK.bit.CMPALINK = 3;
    EPwm5Regs.EPWMXLINK.bit.CMPBLINK = 3;

    //Interrupt vector
    PieVectTable.EPWM5_INT=&epwm5_isr;
    //PWM Synchronization
    EPwm5Regs.TBCTL.bit.PHSEN=TB_ENABLE;
    //EPwm5Regs.TBCTL.bit.PHSDIR=1;//Only for Up-Down
    EPwm5Regs.TBPHS.bit.TBPHS=2;
    EDIS;
}
//End

void PWM_vEPWM5Config(tstEPWMConfig *PWM)
{
    Uint32 u32TBPRDon,u32TBPRDoff;
    EALLOW;
    //Period
    PWM->u32TBPRD=(Uint32)(  (EPWM_CLK_HZ / PWM->u32Freq_hz ) - 1 );//Period Calculation
    u32TBPRDon=( PWM->u32Duty_percent % 100 ) * PWM->u32TBPRD / 100;//On Period
    u32TBPRDoff=( 100 - ( PWM->u32Duty_percent % 100 ) ) * PWM->u32TBPRD / 100;//Off Period
    EPwm5Regs.TBPRD = PWM->u32TBPRD; // EPWM Period
    //Action qualifier setup
    EPwm5Regs.CMPA.bit.CMPA =u32TBPRDon;    // Set compare A value
    EPwm5Regs.CMPB.bit.CMPB =u32TBPRDoff; // Set compare B value
    //Deadband
    //RED FED Calculation
    Uint16 u16DeadBandRED=PWM->u16DeadBandRED_ns/SYS_PERIOD_NS;
    Uint16 u16DeadBandFED=PWM->u16DeadBandFED_ns/SYS_PERIOD_NS;
    EPwm5Regs.DBRED.bit.DBRED=u16DeadBandRED;
    EPwm5Regs.DBFED.bit.DBFED=u16DeadBandFED;
    EDIS;
}

//Start PWM5
void PWM_vEPWM5Start()
{
    EALLOW;
    EPwm5Regs.TBCTL.bit.CTRMODE = TB_COUNT_UP;
    EDIS;
}
//Stop PWM5
void PWM_vEPWM5Stop()
{
    EALLOW;
    EPwm5Regs.TBCTL.bit.CTRMODE = TB_FREEZE;
    EDIS;
}
//Forced Stop
void PWM_vEPWM5TripZoneStop()
{
    EALLOW;
    EPwm5Regs.TZFRC.bit.OST=1;
    EDIS;
}

void PWM_vEPWM5TripZoneClear()
{
    EALLOW;
    EPwm5Regs.TZCLR.bit.OST=1;//Clear OST Flag
    EDIS;
}

//Configure Tripzone for Charge Mode
void PWM_vEPWM5ConverterMode()
{
    EALLOW;
    //Tripzone
    //InputXbarRegs.INPUT1SELECT=4;//Select GPIO4
    EPwm5Regs.TZCLR.bit.OST=1;//Clear OST Flag
    EPwm5Regs.TZSEL.bit.OSHT1=ENABLE;
    EPwm5Regs.TZCTL.bit.TZA = TZ_FORCE_LO;
    EPwm5Regs.TZCTL.bit.TZB = TZ_FORCE_LO;
    EDIS;
}

//Recalculate TBPRD based on new duty value
void PWM_vEPWM5DutyUpdate(tstEPWMConfig *PWM, Uint16 u16newDuty)
{
    Uint32 u32TBPRDon,u32TBPRDoff;
    EALLOW;
    //Update the structure
    PWM->u32Duty_percent=u16newDuty;
    //Recalculate comparator values
    //u32TBPRDon=( PWM->u32Duty_percent % 100 ) / 100 * PWM->u32TBPRD;//On period
    //u32TBPRDoff=( 100- ( PWM->u32Duty_percent % 100 ) ) / 100 * PWM->u32TBPRD;//Off period
    u32TBPRDon=( PWM->u32Duty_percent ) * PWM->u32TBPRD / 100 ;//On period
    u32TBPRDoff=( 100 - ( PWM->u32Duty_percent ) )  * PWM->u32TBPRD / 100;//Off period
    //Update comparator values
    EPwm5Regs.CMPA.bit.CMPA =u32TBPRDon;    // Set compare A value
    EPwm5Regs.CMPB.bit.CMPB =u32TBPRDoff; // Set compare B value
    EDIS;
}

void PWM_vEPWM5DeadbandUpdate(tstEPWMConfig *PWM)
{
    EALLOW;
    Uint16 u16DeadBandRED = PWM->u16DeadBandRED_ns / SYS_PERIOD_NS;
    Uint16 u16DeadBandFED = PWM->u16DeadBandFED_ns / SYS_PERIOD_NS;
    EPwm5Regs.DBRED.bit.DBRED = u16DeadBandRED;
    EPwm5Regs.DBFED.bit.DBFED = u16DeadBandFED;
    EDIS;
}

void PWM_vEPWM5SOCEnable()
{
    EALLOW;
    EPwm5Regs.ETSEL.bit.SOCAEN  = ENABLE;    // Enable SOC
    EPwm5Regs.ETSEL.bit.SOCASEL = SOCZERO;   // Event Select
    EPwm5Regs.ETPS.bit.SOCAPRD = 1;//0-no pulse,1-first pulse,2-2nd pulse
    EDIS;
}

//EPWM5 Interrupt
interrupt void epwm5_isr(void)
{
    EPwm5Regs.ETCLR.bit.INT = 1;
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;
}


