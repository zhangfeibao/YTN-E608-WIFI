#ifndef FIFO_H
#define FIFO_H

#define MaxQueueSize 50
#define QueueArraySize (MaxQueueSize + 1)

typedef uint8_t queueElementT;

typedef struct
{
	queueElementT elements[QueueArraySize];
	volatile uint8_t head;
	volatile uint8_t tail;
}queueCDT;

typedef queueCDT *queueADT;


void InitQueue(queueADT queue);
queueElementT Enqueue(queueADT queue, queueElementT element);
queueElementT Dequeue(queueADT queue);

bool_t QueueIsEmpty(queueADT queue);
bool_t QueueIsFull(queueADT queue);

uint8_t QueueLen(queueADT queue);


#endif
