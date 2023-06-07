#include "MaRTOS.h"

TaskType TASK1;
TaskType TASK2;

void task1(void){
    int c=3;
    c++;
}

void task2(void){
    int c=3;
    c++;
}

int main(void)
{
    TaskType TASK1;
    TaskType TASK2;

    TASK1.TaskEntry_Ptr = task1;
    TASK1.Priority = 1;
    TASK1.autoStart = 1;

    TASK2.TaskEntry_Ptr = task1;
    TASK2.Priority = 1;
    TASK2.autoStart = 1;

    RTOS_Init();
    RTOS_CreateTask(&TASK1);
    RTOS_CreateTask(&TASK2);

	return 0;
}
