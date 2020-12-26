#ifndef _SNSR_PUBLIC_H_
#define _SNSR_PUBLIC_H_

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

// SNSR constants

#define SNSR_DATA_SIZE  64

// types

typedef enum 
{
    SNSR_ID_FIRST = 0,
    SNSR_ID_LAST  = 99,

    SNSR_SIM_ID_FIRST = 100,
    SNSR_SIM_ID_LAST  = 199,

    SNSR_ID_COUNT = SNSR_ID_LAST - SNSR_ID_FIRST + 1,
    SNSR_SIM_ID_COUNT = SNSR_SIM_ID_LAST - SNSR_SIM_ID_FIRST + 1,

} snsr_id_t;

typedef uint64_t snsr_tick_t;

typedef struct 
{
    int ( *read ) ( void*, size_t );            // get sensor data
    uint64_t ( *tick ) ( void );                // get current tick

    bool ( *initialized  ) ( void );            // get initialization status
    bool ( *started      ) ( void );            // get task started status
} snsr_cfg_t;


typedef struct
{
    snsr_id_t id;
    snsr_tick_t tick;
     uint8_t data[ SNSR_DATA_SIZE - sizeof( snsr_id_t ) - sizeof( snsr_tick_t ) ];
} snsr_data_buf_t;
// sizeof == SNSR_DATA_SIZE == 64

// public interface

int snsr_init( snsr_cfg_t* snsr_cfgs, size_t snsr_cnt, snsr_data_buf_t* buf, size_t buf_size );
int snsr_start( void );
int snsr_stop( void );

int snsr_enable( snsr_id_t id );
int snsr_disable( snsr_id_t id );

int snsr_get( snsr_id_t id, void* data, snsr_tick_t* tick );

#endif /* _SNSR_PUBLIC_H_ */