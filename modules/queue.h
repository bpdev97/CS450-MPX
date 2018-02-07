#ifndef _QUEUE_H
#define _QUEUE_H

typedef struct {
int count;
struct pcb* head;
struct pcb* tail;
}readyQueue;

typedef struct {
int count;
struct pcb* head;
struct pcb* tail;
}blockedQueue;

typedef struct {
int count;
struct pcb* head;
struct pcb* tail;
}suspendedReadyQueue;

typedef struct {
int count;
struct pcb* head;
struct pcb* tail;
}suspendedBlockedQueue;

#endif
