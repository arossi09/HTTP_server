#include "queue.h"
#include <stdlib.h>

void queue_create(queue_t *q) {
  q->head = NULL;
  q->tail = NULL;
  q->size = 0;
}

// used for adding value to the queue
// we need to add to the front of the queue
int queue_enqueue(queue_t *q, void *data) {
  queue_node_t *node = malloc(sizeof(queue_node_t));
  if (!node) {
    return -1;
  }
  node->data = data;
  node->next = NULL;
  if (q->tail) {
    q->tail->next = node;
  } else {
    q->head = node;
  }
  q->tail = node;
  q->size++;
  return 0;
}

// used for dequeuing value
void *queue_dequeue(queue_t *q) {
  if (!q->head) {
    return NULL;
  }

  queue_node_t *node = q->head;
  void *data = node->data;

  q->head = node->next;
  if (!q->head) {
    q->tail = NULL;
  }
  q->size--;
	free(node);
  return data;
}

size_t queue_size(queue_t *q) { return q->size; }

int queue_is_empty(queue_t *q) { return q->size == 0; }

int queue_destroy(queue_t *q) {
  queue_node_t *curr = q->head;
  while (curr) {
    queue_node_t *next = curr->next;
    free(curr);
    curr = next;
  }
  q->head = NULL;
  q->tail = NULL;
  q->size = 0;
	return 0;
}
