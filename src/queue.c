#include "queue.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

// Define the structure for the block_queue.
struct block_queue
{
   BlockQueueNode *front; // Pointer to the front of the block_queue.
   BlockQueueNode *rear;  // Pointer to the rear of the block_queue.
};

// Define a structure for a node in the linked list.
struct block_queue_node
{
   BlockCoord *info;
   BlockQueueNode *next; // A pointer to the next node in the linked list.
};

struct block_coord{
    int block_xposition;
    int block_yposition;
};



// Function to create a new empty block_queue.
BlockQueue *q_create()
{
   BlockQueue *q = (BlockQueue *)malloc(sizeof(BlockQueue)); // Allocate memory for the block_queue structure.
   q->front = q->rear = NULL;                 // Initialize the front and rear pointers to NULL, indicating an empty block_queue.
   return q;
}

// Function to check whether the block_queue is empty.
int q_is_empty(BlockQueue *q)
{
   return q->front == NULL;
}

// Function to enqueue a float value into the block_queue.
void q_enqueue(BlockQueue *q, BlockCoord *v)
{
   BlockQueueNode *node = (BlockQueueNode *)malloc(sizeof(BlockQueueNode)); // Allocate memory for a new node.
   node->info = v;
   node->next = NULL;

   if (q_is_empty(q))
      q->front = node;
   else
      q->rear->next = node;

   q->rear = node;
}

// Function to dequeue and return a float value from the block_queue.
BlockCoord *q_dequeue(BlockQueue *q)
{
   assert(!q_is_empty(q));

   BlockCoord *v = q->front->info;
   BlockQueueNode *p = q->front; // Store for removal

   if (q->front != q->rear)
      q->front = q->front->next;
   else
      q->front = q->rear = NULL;

   free(p);
   return v;
}

// Function to free the memory used by the block_queue.
void q_free(BlockQueue *q)
{
   BlockQueueNode *p = q->front;
   while (p != NULL)
   {
      BlockQueueNode *t = p->next; // Store a reference to the next node.
      free(p);                // Free the memory allocated for the current node.
      p = t;                  // Move to the next node.
   }
   free(q); // Free the memory allocated for the block_queue structure itself.
}

void q_print(BlockQueue *q){
    BlockQueueNode *current = q->front;
    while (current!= NULL)
    {
        printf("(%d, %d) ", current->info->block_xposition, current->info->block_yposition);
        current = current->next;
    }
    printf("\n");
}

BlockCoord *create_coord_struct(int x, int y){
   BlockCoord *coord = (BlockCoord *)malloc(sizeof(BlockCoord));
   coord-> block_xposition = x;
   coord-> block_yposition = y;

   return coord;
}