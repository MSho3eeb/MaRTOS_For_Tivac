#ifndef CORTEX_OS_PORTING_H__
#define CORTEX_OS_PORTING_H__

#include "tm4c123gh6pm.h"
#include "MaRTOS_Config.h"

#define OS_PRIVILIGE     __asm volatile(" mrs r3, CONTROL  \n\t"  \
                                        " lsr r3,r3,#0x01   \n\t" \
                                        " lsl r3,r3,#0x01   \n\t" \
                                        " msr CONTROL, r3 ")

#define OS_UNPRIVILIGE   __asm volatile(" mrs r3, CONTROL  \n\t" \
                                        " orr r3,r3,#0x01   \n\t" \
                                        " msr CONTROL, r3 ")

#define OS_SP_TO_PSP     __asm volatile(" mrs r4, CONTROL  \n\t mov r5,#0x02 \n\t orr r4,r4,r5 \n\t msr CONTROL,r4")
#define OS_SP_TP_MSP     __asm volatile(" mrs r4, CONTROL  \n\t mov r5,#0x05 \n\t and r4,r4,r5 \n\t msr CONTROL,r4")

#define OS_SET_PSP(add)              __asm volatile(" msr PSP, %0 "  :: "r" (add) )
#define OS_GET_PSP(add)              __asm volatile(" mrs %0, PSP "  : "=r" (add) )
	
#define OS_SET_MSP(add)              __asm volatile(" mov r4, %0 \n\t  msr MSP, r4 "  :: "r" (add) )
#define OS_GET_MSP(add)              __asm volatile(" mrs r4, MSP \n\t  mov %0, r4 "  : "=r" (add) )


void HW_Init(void);
void Start_Ticker(void);
void Systick_Handler(void);
void TriggerPendSV(void);


#endif /*CORTEX_OS_PORTING_H__*/
