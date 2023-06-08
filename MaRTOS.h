#ifndef MARTOS_H__
#define MARTOS_H__

#include "Scheduler.h"
#include "RTOS_FIFO.h"
#include "MaRTOS_Config.h"
#include "Cortex_OS_Porting.h"



void STARTOS();

RTOS_ErrorIDType RTOS_Init(void);
RTOS_ErrorIDType RTOS_CreateTask(TaskType* Task);
void RTOS_ActivateTask(TaskType* Task);
void RTOS_TerminateTask(TaskType* Task);
void RTOS_TaskWait(uint32 TicksNumber, TaskType* Task);

RTOS_ErrorIDType RTOS_AquireMutex(MutexType* Mutex, TaskType* Task);
void RTOS_ReleaseMutex(MutexType* Mutex);


#endif /*MARTOS_H__*/
