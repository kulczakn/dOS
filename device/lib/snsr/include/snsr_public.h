#ifndef _SNSR_PUBLIC_H_
#define _SNSR_PUBLIC_H_

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "cddp_public.h"

// SNSR constants

#define SNSR_DATA_SIZE     64
#define SNSR_DATA_BUF_SIZE 128

// types

typedef enum 
{
    SNSR_ID_FIRST = 0,

    SNSR_SIM_ID_FIRST = CDDP_SIM_DATA_ID_FIRST,
    SNSR_SIM_ID_LAST  = SNSR_SIM_ID_FIRST + 99,
    
    SNSR_ID_LAST,

    SNSR_ID_COUNT = SNSR_ID_LAST - SNSR_ID_FIRST + 1,
    SNSR_SIM_ID_COUNT = SNSR_SIM_ID_LAST - SNSR_SIM_ID_FIRST + 1,

} snsr_id_t;

typedef uint64_t snsr_tick_t;
typedef int ( *snsr_start_task_t ) ( void* ( *f ) ( void* ) );
typedef int ( *snsr_stop_task_t  ) ( void );

typedef struct 
{
    int ( *read  ) ( void*, size_t, snsr_tick_t* );           
                                                // get sensor data
    uint64_t ( *tick ) ( void );                // get current tick

    bool ( *initialized  ) ( void );            // get initialization status
} snsr_cfg_t;


typedef struct
{
    snsr_id_t   id;
    snsr_tick_t tick;
    bool        enabled;
    uint8_t     data[ SNSR_DATA_SIZE ];
    uint8_t     buf[ SNSR_DATA_BUF_SIZE - SNSR_DATA_SIZE - sizeof( snsr_id_t ) - sizeof( snsr_tick_t ) - sizeof( bool ) ];
} snsr_data_buf_t;
// sizeof == 128

// public interface

int snsr_init( snsr_cfg_t* snsr_cfgs, snsr_start_task_t snsr_start_task, snsr_stop_task_t snsr_stop_task, size_t snsr_cnt, void* buf, size_t buf_size );
int snsr_start( void );
int snsr_stop( void );

int snsr_enable( snsr_id_t id );
int snsr_disable( snsr_id_t id );

int snsr_get( snsr_id_t id, void* data, snsr_tick_t* tick );

#endif /* _SNSR_PUBLIC_H_ */