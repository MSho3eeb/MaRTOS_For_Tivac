#ifndef CORTEX_OS_PORTING_H__
#define CORTEX_OS_PORTING_H__

#include "tm4c123gh6pm.h"

void HW_Init(void);
void trigger_OS_PendSV(void);
void Start_Ticker(void);
void Systick_Handler(void);



#endif /*CORTEX_OS_PORTING_H__*/
