#ifndef RTOS_FIFO_H__
#define RTOS_FIFO_H__

#include <stdint.h>
#include "Scheduler.h"
#include "types.h"


typedef struct task{
    void(*TaskPtr)(void);
    uint32 priority;
    struct task* Next;
}TaskToFIFO;

typedef struct{
    TaskToFIFO* head;
    TaskToFIFO* tail;
    uint32 TasksNum;
}FIFOType;

typedef enum{
    NoError = 0,
    Full,
    Empty,
    Null
}BufferStatusType;


BufferStatusType FIFO_Init(void);
BufferStatusType FIFO_Add(TaskToFIFO* task);
TaskToFIFO* FIFO_Get(void);
TaskToFIFO* FIFO_Peek(void);
uint8 FIFO_IsEmpty(void);
uint32 FIFO_TasksCount(void);



#endif /*RTOS_FIFO_H__*/
