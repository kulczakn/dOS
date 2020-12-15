#ifndef _SYN_MEM_PRIVATE_H_GUARD_
#define _SYN_MEM_PRIVATE_H_GUARD_

#include "pthread.h"

#include "syn_mem_public.h"

// constants

#define SYNC_THREAD_STACK_SIZE 2048

// types

typedef enum {
    SYNC_EVENT_ID_FIRST = 0,

    SYNC_EVENT_MEM_NEW = SYNC_EVENT_ID_FIRST,
    SYNC_EVENT_MEM_GET,
    SYNC_EVENT_MEM_SET,
    SYNC_EVENT_MEM_FREE,

    SYNC_EVENT_MTX_NEW,
    SYNC_EVENT_MTX_LOCK,
    SYNC_EVENT_MTX_UNLOCK,
    SYNC_EVENT_MTX_FREE,

    SYNC_EVENT_SEM_NEW,
    SYNC_EVENT_SEM_WAIT,
    SYNC_EVENT_SEM_POST,
    SYNC_EVENT_SEM_FREE
} sync_event_id_t;


// functions

#endif /* _SYN_MEM_PRIVATE_H_GUARD_ */