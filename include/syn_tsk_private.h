#ifndef _SYN_TSK_PRIVATE_H_GUARD_
#define _SYN_TSK_PRIVATE_H_GUARD_

#include "pthread.h"
#include "string.h"

#include "syn_tsk_public.h"

// constants

#define SYNC_THREAD_STACK_SIZE 2048

#define SYN_TSK_MAX_TICK_FUNCS 16
#define SYN_TSK_MAX_TAG_LEN    63
#define SYN_TSK_MAX_MEM_KEYS   32

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


typedef struct {
    char tag[SYN_TSK_MAX_TAG_LEN];
    syn_tsk_tick_func func;
    syn_mem_key_t     keys[SYN_TSK_MAX_MEM_KEYS];
    uint8_t           keys_len;
} syn_tsk_tcb;

// functions

#endif /* _SYN_TSK_PRIVATE_H_GUARD_ */