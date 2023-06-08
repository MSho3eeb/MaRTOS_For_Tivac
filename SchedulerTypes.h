#ifndef SCHEDULERTYPES_H__
#define SCHEDULERTYPES_H__

#include "types.h"
#include "tm4c123gh6pm.h"

typedef enum{
    OK = 0,
    ReadyQueueInitError,
    TaskExceededStackSize,
    MutexIsReacedToMaxNumberOfUsers
}RTOS_ErrorIDType;

typedef enum{
		Init = 0,
    Suspend,
    Running,
    Waiting,
    Ready
}TaskStateType;

typedef enum{
    Disable = 0,
    Enable
}BlockingType;

typedef struct{
    BlockingType Blocking;
    uint32 TicksCount;
}TimingWaitingType;

typedef struct{
    uint32 StackSize;
    uint8  Priority;
		uint8  OrigiPriority;
    void (*TaskEntry_Ptr)(void); // function pointer
    uint8  autoStart;
    uint32 _S_PSP_Task;
    uint32 _E_PSP_Task;
    uint32* Current_PSP;
    uint8 TaskName[30];
    TaskStateType TaskState;
    TimingWaitingType TimingWaiting;
}TaskType;

typedef struct{
    uint8 * PayLoadPtr;
    uint32  PayLoadSize;
    TaskType * CurrentUserPtr;
    TaskType * NextUserPtr;
    uint8  MutexName[30];
}MutexType;


#endif /*SCHEDULERTYPES_H__*/
