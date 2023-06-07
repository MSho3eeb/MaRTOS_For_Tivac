#include "RTOS_FIFO.h"

FIFOType RTOS_FIFO;

BufferStatusType FIFO_Init(void){
    RTOS_FIFO.head = NULL;
    RTOS_FIFO.tail = NULL;
    RTOS_FIFO.TasksNum = 0;

    return NoError;
}

BufferStatusType FIFO_Add(TaskToFIFO* task){
    task->Next = NULL;
    if(RTOS_FIFO.tail != NULL){
        RTOS_FIFO.tail->Next = task;
    }else{
        RTOS_FIFO.head = task;
    }
    RTOS_FIFO.tail = task;
    RTOS_FIFO.TasksNum++;

    return NoError;
}
TaskToFIFO* FIFO_Get(void){
    TaskToFIFO* task = RTOS_FIFO.head;
    if(task != NULL){
        RTOS_FIFO.head = task->Next;
        if(RTOS_FIFO.head == NULL){
            RTOS_FIFO.tail = NULL;
        }
        RTOS_FIFO.TasksNum--;
    }
    return task;
}
TaskToFIFO* FIFO_Peek(void){
    return RTOS_FIFO.head;
}
uint8 FIFO_IsEmpty(void){
    return (RTOS_FIFO.head == NULL);
}
uint32 FIFO_TasksCount(void){
    return RTOS_FIFO.TasksNum;
}















