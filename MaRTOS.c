#include "MaRTOS.h"

FIFOType ReadyQueue;
TaskType* ReadyQueueFIFO[MAXTASKS];
TaskType* CurrentTask;
TaskType* NextTask;
uint8 TaskCount;



RTOS_ErrorIDType RTOS_Init(void){
    FIFO_Init(&ReadyQueue, ReadyQueueFIFO, MAXTASKS);
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
    FIFO_Add(Task);
    Task->TaskState = Ready;
}
void RTOS_TerminateTask(TaskType* Task){
    Task->TaskState = Suspend;
}
void RTOS_TaskWait(uint32 TicksNumber, TaskType* Task){
    Task->TaskState = Waiting;
    Task->TimingWaiting.TicksCount = TicksNumber;
    Task->TimingWaiting.Blocking = Enable;
}

//RTOS_ErrorIDType RTOS_AquireMutex(MutexType* Mutex, TaskType* Task){
//
//}
//void RTOS_ReleaseMutex(MutexType* Mutex){
//
//}
