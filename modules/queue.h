#ifndef _QUEUE_H
#define _QUEUE_H

typedef struct readyQueue {
int count;
struct pcb* head;
struct pcb* tail;
};

typedef struct blockedQueue {
int count;
struct pcb* head;
struct pcb* tail;
};

typedef struct suspendedReadyQueue {
int count;
struct pcb* head;
struct pcb* tail;
};

typedef struct suspendedBlockedQueue {
int count;
struct pcb* head;
struct pcb* tail;
};

#endif
