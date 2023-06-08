#include "MaRTOS.h"

volatile FIFOType ReadyQueue;
TaskType* ReadyQueueFIFO[MAXTASKS];
TaskType* CurrentTask;
TaskType* NextTask;
uint8 TaskCount;
TaskType RTOS_IdleTask;
TaskType RTOS_IdleTask1;

struct{
    TaskType* OSTasks[MAXTASKS];
    uint32 _S_MSP_TASK;
    uint32 _E_MSP_TASK;
    uint32 PSP_Task_Locator;
    uint32 NoOfActiveTasks;
    TaskType* CurrentTask;
    TaskType* NextTask;
    enum{
        OSSuspend = 0,
        OSRunnig
    }OSMODE;
}OS_Control;

typedef enum{
    SVC_ActivateTask = 0,
    SVC_TerminateTask,
    SVC_TaskWaiting,
}SVC_ID;

void RTOS_IDLE(void){
    while(1){
				OS_Control.NextTask = &RTOS_IdleTask1;
				__asm(" wfe");
    }
}
void RTOS_IDLE1(void){
    while(1){
        OS_Control.NextTask = &RTOS_IdleTask;
				__asm(" wfe");
    }
}

void RTOS_Create_MainStack(void){
    OS_Control._E_MSP_TASK = (uint32)0x20000400; //&
    OS_Control._S_MSP_TASK = OS_Control._E_MSP_TASK + MAINSTACKSIZE;
    OS_Control.PSP_Task_Locator = (OS_Control._E_MSP_TASK - 8);
}
void STARTOS(){
		FIFO_Init(&ReadyQueue, ReadyQueueFIFO, MAXTASKS);
    OS_Control.OSMODE = OSRunnig;
    OS_Control.CurrentTask = &RTOS_IdleTask;
		OS_Control.NextTask = &RTOS_IdleTask1;
    RTOS_ActivateTask(&RTOS_IdleTask);

    Start_Ticker();
		OS_Control.CurrentTask->Current_PSP = OS_Control.CurrentTask->Current_PSP;
		OS_SET_PSP(OS_Control.CurrentTask->Current_PSP);
		OS_SP_TO_PSP;
		OS_UNPRIVILIGE;
		RTOS_IdleTask.TaskEntry_Ptr();

}


RTOS_ErrorIDType RTOS_Init(void){
    HW_Init();
    OS_Control.OSMODE = OSSuspend;
	  RTOS_Create_MainStack();
    RTOS_IdleTask.Priority = 255;
    RTOS_IdleTask.TaskEntry_Ptr = RTOS_IDLE;
    RTOS_IdleTask.StackSize = 300;
		RTOS_IdleTask1.Priority = 255;
    RTOS_IdleTask1.TaskEntry_Ptr = RTOS_IDLE1;
    RTOS_IdleTask1.StackSize = 300;

    RTOS_CreateTask(&RTOS_IdleTask);
		RTOS_CreateTask(&RTOS_IdleTask1);

    return OK;
}

void RTOS_Create_TaskStack(TaskType* Task){
    Task->Current_PSP = (uint32*)Task->_S_PSP_Task;

    Task->Current_PSP--;
    *(Task->Current_PSP) = (uint32)(1<<24); //PSR

    Task->Current_PSP--;
    *(Task->Current_PSP) = (uint32)(Task->TaskEntry_Ptr);//PC

    Task->Current_PSP--;
    *(Task->Current_PSP) = (uint32)0xFFFFFFFD;//LR
		
		for(int i = 0; i<13 ; i++){
			Task->Current_PSP--;
			*(Task->Current_PSP) = 0x00; //R12
		}
			
}

RTOS_ErrorIDType RTOS_CreateTask(TaskType* Task){

    Task->_S_PSP_Task = OS_Control.PSP_Task_Locator;
    Task->_E_PSP_Task = (Task->_S_PSP_Task - Task->StackSize);
    OS_Control.PSP_Task_Locator = (Task->_E_PSP_Task - 8);

    RTOS_Create_TaskStack(Task);

    OS_Control.OSTasks[OS_Control.NoOfActiveTasks] = Task;
    OS_Control.NoOfActiveTasks++;

    Task->TaskState = Suspend;

    TaskCount++;
    return OK;
}

void SORT(void){
    uint32 i,j,n;
    TaskType* temp;
    n = OS_Control.NoOfActiveTasks;

    for(i = 0; i < n-1; i++){
        for(j = 0; j < n-i-1; j++){
            if(OS_Control.OSTasks[j]->Priority > OS_Control.OSTasks[j + 1]->Priority){
                temp = OS_Control.OSTasks[j];
                OS_Control.OSTasks[j] = OS_Control.OSTasks[j+1];
                OS_Control.OSTasks[j+1] = temp;
            }
        }
    }
}

void RTOS_Update_Schedule_Tables(void){
    TaskType* temp = NULL;
    TaskType* TaskPtr;
    TaskType* NextTaskPtr;
    uint32 i = 0;

    SORT();

    while(FIFO_Get() != NULL);

    while(i<OS_Control.NoOfActiveTasks){
        TaskPtr = OS_Control.OSTasks[i];
        NextTaskPtr = OS_Control.OSTasks[i+1];
        if(TaskPtr->TaskState != Suspend){
            if(NextTaskPtr->TaskState == Suspend){
                FIFO_Add(TaskPtr);
                TaskPtr->TaskState = Ready;
                break;
            }
            if(TaskPtr->Priority < NextTaskPtr->Priority){
                FIFO_Add(TaskPtr);
                TaskPtr->TaskState = Ready;
                break;
            }else if(TaskPtr->Priority == NextTaskPtr->Priority){
                FIFO_Add(TaskPtr);
                TaskPtr->TaskState = Ready;
            }else if(TaskPtr->Priority > NextTaskPtr->Priority){
                break;
            }
        }
        i++;
    }

}

void WhatIsNext(void){
    if((ReadyQueue.counter == 0) && (OS_Control.CurrentTask->TaskState != Suspend)){
        OS_Control.CurrentTask->TaskState = Running;
        FIFO_Add(OS_Control.CurrentTask);
        OS_Control.NextTask = OS_Control.CurrentTask;
    }else{
        OS_Control.NextTask = FIFO_Get();
        OS_Control.NextTask->TaskState = Running;
        if((OS_Control.CurrentTask->Priority == OS_Control.NextTask->Priority) && (OS_Control.CurrentTask->TaskState != Suspend)){
            FIFO_Add(OS_Control.CurrentTask);
            OS_Control.CurrentTask->TaskState = Ready;
        }
    }
}

void OS_SVC(int* StackFrame){

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

void PendSV_Handler(void){
	RTOS_Update_Schedule_Tables();
	WhatIsNext();
	OS_GET_PSP(OS_Control.CurrentTask->Current_PSP);
	
	OS_Control.CurrentTask->Current_PSP--;
	__asm volatile(" mov %0,r4 " : "=r" (*(OS_Control.CurrentTask->Current_PSP)));
	
	OS_Control.CurrentTask->Current_PSP--;
	__asm volatile(" mov %0,r5 " : "=r" (*(OS_Control.CurrentTask->Current_PSP)));
	
	OS_Control.CurrentTask->Current_PSP--;
	__asm volatile(" mov %0,r6 " : "=r" (*(OS_Control.CurrentTask->Current_PSP)));
	
	OS_Control.CurrentTask->Current_PSP--;
	__asm volatile(" mov %0,r7 " : "=r" (*(OS_Control.CurrentTask->Current_PSP)));
	
	OS_Control.CurrentTask->Current_PSP--;
	__asm volatile(" mov %0,r8 " : "=r" (*(OS_Control.CurrentTask->Current_PSP)));
	
	OS_Control.CurrentTask->Current_PSP--;
	__asm volatile(" mov %0,r9 " : "=r" (*(OS_Control.CurrentTask->Current_PSP)));
	
	OS_Control.CurrentTask->Current_PSP--;
	__asm volatile(" mov %0,r10 " : "=r" (*(OS_Control.CurrentTask->Current_PSP)));
	
	OS_Control.CurrentTask->Current_PSP--;
	__asm volatile(" mov %0,r11 " : "=r" (*(OS_Control.CurrentTask->Current_PSP)));
	
	if(OS_Control.NextTask == NULL){
		
	}else{
		OS_Control.CurrentTask = OS_Control.NextTask;
		OS_Control.NextTask = NULL;
	}
	
	__asm volatile(" mov r11, %0" : : "r"(*(OS_Control.CurrentTask->Current_PSP)));
	OS_Control.CurrentTask->Current_PSP++;
	
	__asm volatile(" mov r10, %0" : : "r"(*(OS_Control.CurrentTask->Current_PSP)));
	OS_Control.CurrentTask->Current_PSP++;
	
	__asm volatile(" mov r9, %0" : : "r"(*(OS_Control.CurrentTask->Current_PSP)));
	OS_Control.CurrentTask->Current_PSP++;
	
	__asm volatile(" mov r8, %0" : : "r"(*(OS_Control.CurrentTask->Current_PSP)));
	OS_Control.CurrentTask->Current_PSP++;
	
	__asm volatile(" mov r7, %0" : : "r"(*(OS_Control.CurrentTask->Current_PSP)));
	OS_Control.CurrentTask->Current_PSP++;
	
	__asm volatile(" mov r6, %0" : : "r"(*(OS_Control.CurrentTask->Current_PSP)));
	OS_Control.CurrentTask->Current_PSP++;
	
	__asm volatile(" mov r5, %0" : : "r"(*(OS_Control.CurrentTask->Current_PSP)));
	OS_Control.CurrentTask->Current_PSP++;
	
	__asm volatile(" mov r4, %0" : : "r"(*(OS_Control.CurrentTask->Current_PSP)));
	OS_Control.CurrentTask->Current_PSP++;
	
	OS_SET_PSP(OS_Control.CurrentTask->Current_PSP);
	__asm volatile("BX LR");

}
