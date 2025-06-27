/*
 * circular_queue.h
 *
 *  Created on: 20-Nov-2024
 *      Author: user
 */

#ifndef CIRCULAR_QUEUE_H
#define CIRCULAR_QUEUE_H

#include <stdint.h>  // For uint8_t
#include <stdbool.h> // For bool type

// Define the size of the circular buffer
#define BUFFER_SIZE 512  // You can modify this size based on your application

// Circular Queue structure
typedef struct
{
    uint8_t buffer[BUFFER_SIZE];  // Storage buffer
    uint16_t head;               // Index of the next write position
    uint16_t tail;               // Index of the next read position
} CircularQueue;

// Function prototypes

void CircularQueue_Init(CircularQueue *queue);
bool CircularQueue_IsFull(CircularQueue *queue);
bool CircularQueue_IsEmpty(CircularQueue *queue);
bool CircularQueue_Enqueue(CircularQueue *queue, uint8_t data);
bool CircularQueue_Dequeue(CircularQueue *queue, uint8_t *data);

#endif // CIRCULAR_QUEUE_H
