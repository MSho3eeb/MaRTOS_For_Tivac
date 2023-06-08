#include "Cortex_OS_Porting.h"

volatile int d = 0;

void HardFault_Handler(void){
    while(1);
}
void MemManage_Handler(void){
    while(1);
}
void BusFault_Handler(void){
    while(1);
}
void UsageFault_Handler(void){
    while(1);
}
__attribute ((naked)) void SVC_Handler(){
    __asm  (" tst lr, #4 \n\t"
            " ITE EQ \n\t"
            " mrseq r0, MSP \n\t"
            " mrsne r0, PSP \n\t"
           // " B OS_SVC "
            );
}

void TriggerPendSV(void){
    OS_PRIVILIGE;
    NVIC_INT_CTRL_R |= 1<<28;
    OS_UNPRIVILIGE;
}

void HW_Init(void){

    OS_PRIVILIGE;

    NVIC_SYS_PRI3_R |= (2<<21); // PENDSV
    NVIC_SYS_PRI3_R |= (1<<29); // SYSTICK
    NVIC_ST_CTRL_R = (1<<1) | (1<<2);
	
	//	OS_UNPRIVILIGE;

}
void Start_Ticker(void){
		OS_PRIVILIGE;

    NVIC_ST_RELOAD_R = (SYSTEMCLK / TICKFREQ) - 1;
    NVIC_ST_CTRL_R |= (1<<0);
	  //OS_UNPRIVILIGE;

}
void SysTick_Handler(void){
    TriggerPendSV();
}

