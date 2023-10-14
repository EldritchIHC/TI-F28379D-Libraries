/*
 * NTC_down.c
 *
 *  Created on: Sep 19, 2022
 *      Author: damiem1
 */


#include "F28x_Project.h"
#include "NTC.h"
#include "NTC_DATA.h"


int16 NTC__i16TempDatLUT[TEMP_SENSOR_DATA_POINTS][2] = TEMP_SENSOR_DATA; //int16: -32768 to 32767 //some temperatures are negative


float NTC_i16GetTemperature(Uint16 u16Code)    //this function return float data
    {

    float fTemperatureValue;  // the result of function
    float fRamp;

    Uint16 u16CounterFirst = 0;                           //first element, 0 position
    Uint16 u16CounterMiddle = TEMP_SENSOR_DATA_POINTS/2; //191:2=95
    Uint16 u16CounterLast = TEMP_SENSOR_DATA_POINTS-1;   //last element, 190 position
    int16 i16Temp;   //the temperature is with sign


    while(abs(u16CounterFirst - u16CounterLast) > 1)
        {

           if(NTC__i16TempDatLUT[u16CounterMiddle][NTC__nCodeIndex] < u16Code)
            {
                u16CounterLast = u16CounterMiddle;
            }
            else
            {
                u16CounterFirst = u16CounterMiddle;
            }

           u16CounterMiddle = (u16CounterFirst + u16CounterLast)/2;
        }

       fRamp = (float)abs(NTC__i16TempDatLUT[u16CounterFirst][NTC__nTempIndex] - NTC__i16TempDatLUT[u16CounterLast][NTC__nTempIndex])/
                (float)abs(NTC__i16TempDatLUT[u16CounterFirst][NTC__nCodeIndex] - NTC__i16TempDatLUT[u16CounterLast][NTC__nCodeIndex]);

       fTemperatureValue = (NTC__i16TempDatLUT[u16CounterFirst][NTC__nTempIndex] < NTC__i16TempDatLUT[u16CounterLast][NTC__nTempIndex]) ? (NTC__i16TempDatLUT[u16CounterFirst][NTC__nTempIndex]) : (NTC__i16TempDatLUT[u16CounterLast][NTC__nTempIndex]);

       i16Temp = (NTC__i16TempDatLUT[u16CounterFirst][NTC__nCodeIndex] < NTC__i16TempDatLUT[u16CounterLast][NTC__nCodeIndex]) ? (NTC__i16TempDatLUT[u16CounterFirst][NTC__nCodeIndex]) : (NTC__i16TempDatLUT[u16CounterLast][NTC__nCodeIndex]);
       i16Temp = abs(i16Temp - (int16)u16Code);

       fTemperatureValue += i16Temp * fRamp;


        return fTemperatureValue;
        }
