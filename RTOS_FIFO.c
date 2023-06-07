#include "RTOS_FIFO.h"

FIFOType* RTOS_FIFO;

BufferStatusType FIFO_Init(FIFOType* fifo, TaskType** buff, uint32 length){
    RTOS_FIFO = fifo;
    if(buff == NULL){
        return Null;
    }

    RTOS_FIFO->base = buff;
    RTOS_FIFO->head = fifo->base;
    RTOS_FIFO->tail = fifo->base;
    RTOS_FIFO->length = length;
    RTOS_FIFO->counter = 0;

    return NoError;
}

BufferStatusType FIFO_Add(TaskType* task){
    if((RTOS_FIFO->base == NULL) || (RTOS_FIFO->length == 0)){
        return Null;
    }

    if(RTOS_FIFO->counter == RTOS_FIFO->length){
        return Full;
    }

    *(RTOS_FIFO->tail) = task;
    RTOS_FIFO->tail++;
    RTOS_FIFO->counter++;

    return NoError;
}
TaskType* FIFO_Get(void){
    TaskType* task;
    if((RTOS_FIFO->base == NULL) || (RTOS_FIFO->length == 0)){
        return NULL;
    }
    if(RTOS_FIFO->head == RTOS_FIFO->tail){
        return NULL;
    }
    task = *(RTOS_FIFO->head);
    RTOS_FIFO->counter--;
    RTOS_FIFO->head++;

    return task;

}
TaskType* FIFO_Peek(void){
    return *(RTOS_FIFO->head);
}
uint8 FIFO_IsEmpty(void){
    return (RTOS_FIFO->head == NULL);
}
uint32 FIFO_TasksCount(void){
    return RTOS_FIFO->counter;
}















