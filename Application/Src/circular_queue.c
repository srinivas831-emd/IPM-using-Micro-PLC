/*
 * circular_queue.c
 *
 *  Created on: 20-Nov-2024
 *      Author: user
 */


#include "circular_queue.h"
#include "com_terminal.h"

void CircularQueue_Init(CircularQueue *queue) {
    queue->head = 0;
    queue->tail = 0;
}

bool CircularQueue_IsFull(CircularQueue *queue) {
    return ((queue->head + 1) % BUFFER_SIZE) == queue->tail;
}

bool CircularQueue_IsEmpty(CircularQueue *queue) {
    return queue->head == queue->tail;
}

bool CircularQueue_Enqueue(CircularQueue *queue, uint8_t data) {
    if (CircularQueue_IsFull(queue)) {
        return false;  // Queue is full
    }
    queue->buffer[queue->head] = data;
    queue->head = (queue->head + 1) % BUFFER_SIZE;
    return true;
}

bool CircularQueue_Dequeue(CircularQueue *queue, uint8_t *data) {
    if (CircularQueue_IsEmpty(queue)) {
        return false;  // Queue is empty
    }
    *data = queue->buffer[queue->tail];
    queue->tail = (queue->tail + 1) % BUFFER_SIZE;
    return true;
}
