#ifndef QUEUE_H
#define QUEUE_H
#include <stdio.h>

typedef struct queue_node_t {
  void *data;
  struct queue_node_t *next;
} queue_node_t;

typedef struct {
  queue_node_t *tail;
  queue_node_t *head;
  size_t size;
} queue_t;

// used for initilize the queue
void queue_create(queue_t *q);

// used for adding value to the queue
int queue_enqueue(queue_t *q, void *data);

// used for dequeuing value
void *queue_dequeue(queue_t *q);

size_t queue_size(queue_t *q);

int queue_is_empty(queue_t *q);

int queue_destroy(queue_t *q);

#endif
