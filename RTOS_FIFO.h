#ifndef RTOS_FIFO_H__
#define RTOS_FIFO_H__

#include <stdint.h>
#include "Scheduler.h"
#include "types.h"


typedef struct{
    uint32 counter;
    TaskType** head;
    TaskType** tail;
    TaskType** base;
    uint32 length;
}FIFOType;

typedef enum{
    NoError = 0,
    Full,
    Empty,
    Null
}BufferStatusType;


BufferStatusType FIFO_Init(FIFOType* fifo, TaskType** buff, uint32 length);
BufferStatusType FIFO_Add(TaskType* task);
TaskType* FIFO_Get(void);
TaskType* FIFO_Peek(void);
uint8 FIFO_IsEmpty(void);
uint32 FIFO_TasksCount(void);



#endif /*RTOS_FIFO_H__*/
