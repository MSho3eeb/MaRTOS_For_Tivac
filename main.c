#include "tm4c123gh6pm.h"

#include "MaRTOS.h"


TaskType TASK1;
TaskType TASK2;
TaskType TASK3;
TaskType TASK4;
uint32 c = 0;

void task1(void){
    //TASK4.Priority = 1;
		//TASK1.Priority = 4;
		while(1){
			c++;
		}
}

void task2(void){
    //TASK1.Priority = 1;
		//TASK2.Priority = 2;
		while(1){
			c++;
		}
}

void task3(void){
    //TASK2.Priority = 1;
		//TASK3.Priority = 3;
		while(1){
			GPIO_PORTF_DATA_R ^= 0x04;
			RTOS_TaskWait(2000, &TASK3);
		}
}	

void task4(void){
		while(1){
			c++;
			GPIO_PORTF_DATA_R ^= 0x02;
			RTOS_TaskWait(1000, &TASK4);
		}
}

extern volatile FIFOType ReadyQueue;
extern TaskType* ReadyQueueFIFO[MAXTASKS];


int main(void)
	{
		SYSCTL_RCGCGPIO_R |= (1<<5);
		GPIO_PORTF_CR_R = 0x0E;
		GPIO_PORTF_DIR_R = 0x0E;
		GPIO_PORTF_DEN_R = 0x0E;

		
    RTOS_Init();
		MTaskCreate(&TASK1, task1, 4, 128, "Task1");
		MTaskCreate(&TASK2, task2, 3, 128, "Task2");
		MTaskCreate(&TASK3, task3, 1, 128, "Task3");
		MTaskCreate(&TASK4, task4, 1, 128, "Task4");
		
		
    STARTOS();
		
    while(1){

    }

		return 0;
}