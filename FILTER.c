/*
 * FILTER.c
 *
 *  Created on: 15 sept. 2022
 *      Author: Paul
 */

/* Project Headers */
#include "F28x_Project.h"

/* System Headefiles*/
#include <stdlib.h>

/* Own Headerfiles */
#include "FILTER.h"

/* Extern Headerfiles */

/* Function Prototypes */
Uint16 FILTER_u16FIlter(tstFILTERConfig *FILTER, Uint16 u16Input);

/* Global Variables */
tstFILTERConfig stFILTER1 = FILTER_INIT;
tstFILTERConfig stFILTER2 = FILTER_INIT;
tstFILTERConfig stFILTER3 = FILTER_INIT;

Uint16 FILTER_u16Filter(tstFILTERConfig *FILTER, Uint16 u16Input)
{
    FILTER->u16Out = ( (u16Input + (FILTER->u16InOld * 7) ) >> 3 );
    FILTER->u16InOld = u16Input;
    return FILTER->u16Out;
}
