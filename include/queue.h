#ifndef QUEUE_H
#define QUEUE_H

typedef struct block_queue BlockQueue;
typedef struct block_queue_node BlockQueueNode;
typedef struct block_coord BlockCoord;

BlockQueue *q_create();

int q_is_empty(BlockQueue *q);

void q_enqueue(BlockQueue *q, BlockCoord *v);

BlockCoord *q_dequeue(BlockQueue *q);

void q_free(BlockQueue *q);

void q_print(BlockQueue *q);

BlockCoord *create_coord_struct(int x, int y);

#endif 