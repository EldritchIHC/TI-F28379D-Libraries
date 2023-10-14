/*
 * FILTER.h
 *
 *  Created on: 15 sept. 2022
 *      Author: Paul
 */

#ifndef FILTER_H_
#define FILTER_H_
//#define SYSTEM_CLOCK_HZ 200000000
//#define SYSTEM_PERIOD_NS 5FL
//de luat in calcul prescaler

typedef struct{
     Uint16 u16InOld;
     Uint16 u16Out;
}tstFILTERConfig;

#define FILTER_INIT { 0, 0 }

Uint16 FILTER_u16Filter(tstFILTERConfig *FILTER, Uint16 u16Input);

extern tstFILTERConfig stFILTER1;
extern tstFILTERConfig stFILTER2;
extern tstFILTERConfig stFILTER3;

#endif /* FILTER_H_ */
