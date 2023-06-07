#include "MaRTOS.h"

TaskToFIFO* CurrentTask;
TaskToFIFO* NextTask;
uint8 TaskCount;

RTOS_ErrorIDType RTOS_Init(void){
    FIFO_Init();
    return OK;
}
RTOS_ErrorIDType RTOS_CreateTask(TaskType* Task){
    Task->_E_PSP_Task = (uint32)Task + (Task->StackSize);
    Task->_S_PSP_Task = Task->_E_PSP_Task - 32*4;
    Task->Current_PSP = (uint32*)(Task->_S_PSP_Task);

    *(Task->Current_PSP + 24) = (uint32)(Task->TaskEntry_Ptr);
    *(Task->Current_PSP + 25) = (uint32)0x01000000;

    if(Task->autoStart == 1){
        RTOS_ActivateTask(Task);
    }

    TaskCount++;
    return OK;
}
void RTOS_ActivateTask(TaskType* Task){

}
void RTOS_TerminateTask(TaskType* Task){

}
void RTOS_TaskWait(uint32 TicksNumber, TaskType* Task){

}

RTOS_ErrorIDType RTOS_AquireMutex(MutexType* Mutex, TaskType* Task){

}
void RTOS_ReleaseMutex(MutexType* Mutex){

}
