#include "heads.h"

void InitQueue(queueADT queue)
{
	queue->head = queue->tail = 0;
}

bool_t QueueIsEmpty(queueADT queue)
{
	return (bool_t)(queue->head == queue->tail);
}

bool_t QueueIsFull(queueADT queue)
{
	return (bool_t)((queue->tail + 1) % QueueArraySize == queue->head);
}

uint8_t QueueLen(queueADT queue)
{
	return ((QueueArraySize + queue->tail - queue->head) % QueueArraySize);
}

queueElementT Enqueue(queueADT queue, queueElementT element)
{
	if (!QueueIsFull(queue))
	{
		queue->elements[queue->tail] = element;
		queue->tail = (queue->tail + 1) % QueueArraySize;
	}

	return element;
}

queueElementT Dequeue(queueADT queue)
{
	queueElementT result;

	result = 0;
	if (!QueueIsEmpty(queue))
	{
		result = queue->elements[queue->head];
		queue->head = (queue->head + 1) % QueueArraySize;
	}
	
	return result;
}