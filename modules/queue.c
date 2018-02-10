#include "queue.h"
#include <system.h>

QUEUE *ready = NULL;
QUEUE *readySuspended = NULL;
QUEUE *blocked = NULL;
QUEUE *blockedSuspended = NULL;