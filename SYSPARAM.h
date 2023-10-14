/*
 * SYSPARAM.h
 *
 *  Created on: 12 feb. 2023
 *      Author: PAul
 */

#ifndef SYSPARAM_H_
#define SYSPARAM_H_
/*Defines*/
#define SYS_CLK_HZ 200000000UL
#define EPWM_CLK_HZ (SYS_CLK_HZ>>1)
/*Divide by 2*/
#define SYS_PERIOD_NS 5
#define MS_TO_NS 1000000
#define UINT32MAX 4294967295
#define ENABLE 1
#define DISABLE 0
#define DEBUG
#define BUCK
//#define BOOST
/*ADC LSB*/
#define LSB 0.000732421875/* 3/4096 */
/*Temperature Measurements masks*/
#define TEMPMEAS1 0x0001
#define TEMPMEAS2 0x0002
#define TEMPMEAS3 0x0004
/*Critical temperature*/
#define TEMPCRIT 200
/*Middle Capacitor precharge voltage*/
//#define CMID_BUCK 24.0F
#define CMID_BUCK 4.0F
#define CMID_BUCK_LOW 3.0F
#define CMID_BUCK_HIGH 6.0F
#define CMID_BOOST 48.0F
/*Converter phases*/
#define CONVERTER_OFF       0
#define CONVERTER_PRECHARGE 1
#define CONVERTER_ON        2
/*Duty percents*/
#define DUTYMAX 90
#define DUTYMIN 10
#define DUTYSTART 50
#define DUTYCHARGE 5
/*Converter Switching Frequency*/
#define HC_SWITCHING_FREQUENCY 500000UL
/*ADC Conversions */
#define CURRENT12V_SLOPE 16.61
#define CURRENT12V_SLOPE_INV 0.0602
#define CURRENT12V_OFFSET 3373.35

#define VOLTAGE12V_SLOPE 256.35
#define VOLTAGE12V_SLOPE_INV 0.0039
#define VOLTAGE12V_OFFSET -3.35

#define CURRENT48V_SLOPE 11.0
#define CURRENT48V_SLOPE_INV 0.09
#define CURRENT48V_OFFSET 3354.12

#define VOLTAGE48V_SLOPE 67.58
#define VOLTAGE48V_SLOPE_INV 0.0148
#define VOLTAGE48V_OFFSET -1.67

#define CMID_SLOPE 128.36
#define CMID_SLOPE_INV 0.00779
#define CMID_OFFSET -24.15
/*Structures*/
/*Function prototypes*/
/*Variables*/
extern Uint16 u16TempStatus;
extern Uint16 u16ConverterStatus;
extern Uint16 u16Strobe;
extern float fVoltage12VMeasurement;
extern float fCurrent12VMeasurement;
extern float fCMidMeasurement;

#endif /* SYSPARAM_H_ */
