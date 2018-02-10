#ifndef _QUEUE_H
#define _QUEUE_H

typedef struct queue{
int count;
struct pcb *head;
struct pcb *tail;
} QUEUE;

extern QUEUE *ready;
extern QUEUE *readySuspended;
extern QUEUE *blocked;
extern QUEUE *blockedSuspended;

#endif
