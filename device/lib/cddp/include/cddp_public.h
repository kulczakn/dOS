#ifndef _CDDP_PUBLIC_H_
#define _CDDP_PUBLIC_H_

#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <arpa/inet.h> 
#include <sys/socket.h> 

// CDDP constants

#define CDDP_DATA_SIZE  64

// CDDP types

typedef enum 
{

    CDDP_DATA_ID_FIRST = 0,

    CDDP_SYS_DATA_ID_FIRST = CDDP_DATA_ID_FIRST,
    CDDP_SYS_DATA_ID_LAST  = 99,

    CDDP_PRJ_DATA_ID_FIRST = 100,
    CDDP_PRJ_DATA_ID_LAST  = 199,

    CDDP_SIM_DATA_ID_FIRST = 200,
    CDDP_SIM_DATA_ID_LAST  = 299,

    CDDP_DATA_ID_LAST,

    CDDP_SYS_DATA_ID_COUNT = CDDP_SYS_DATA_ID_LAST - CDDP_SYS_DATA_ID_FIRST + 1,
    CDDP_PRJ_DATA_ID_COUNT = CDDP_PRJ_DATA_ID_LAST - CDDP_PRJ_DATA_ID_FIRST + 1,
    CDDP_SIM_DATA_ID_COUNT = CDDP_SIM_DATA_ID_LAST - CDDP_SIM_DATA_ID_FIRST + 1,
    CDDP_DATA_ID_COUNT = CDDP_DATA_ID_LAST - CDDP_DATA_ID_FIRST

} cddp_data_id_t;


// displayable data formats
typedef enum {
    CDDP_DATA_FRMT_FIRST = 0,

    CDDP_DATA_FMRT_UINT8 = CDDP_DATA_FRMT_FIRST,
    CDDP_DATA_FMRT_UINT16,
    CDDP_DATA_FMRT_UINT32,
    CDDP_DATA_FMRT_UINT64,

    CDDP_DATA_FMRT_INT8,
    CDDP_DATA_FMRT_INT16,
    CDDP_DATA_FMRT_INT32,
    CDDP_DATA_FMRT_INT64,

    CDDP_DATA_FRMT_CHAR,
    CDDP_DATA_FMRT_CHAR8,
    CDDP_DATA_FMRT_CHAR16,
    CDDP_DATA_FMRT_CHAR32,
    CDDP_DATA_FMRT_CHAR64,

    CDDP_DATA_FRMT_FLOAT,
    CDDP_DATA_FRMT_DOUBLE,
    CDDP_DATA_FRMT_BOOL,

    CDDP_DATA_FRMT_LAST,
    CDDP_DATA_FRMT_COUNT = CDDP_DATA_FRMT_LAST - CDDP_DATA_FRMT_FIRST
} cddp_data_frmt_t;


// monitor display formats
typedef enum 
{
    CDDP_MNTR_FRMT_FIRST = 0,

    CDDP_MNTR_FRMT_RAW = CDDP_MNTR_FRMT_FIRST,

    CDDP_MNTR_FRMT_DEGREE,
    CDDP_MNTR_FRMT_PERCENT,
    CDDP_MNTR_FRMT_TEXT,

    CDDP_MNTR_FRMT_DECIMAL1,
    CDDP_MNTR_FRMT_DECIMAL2,
    CDDP_MNTR_FRMT_DECIMAL3,
    CDDP_MNTR_FRMT_DECIMAL4,
    CDDP_MNTR_FRMT_DECIMAL5,
    CDDP_MNTR_FRMT_DECIMAL6,
    CDDP_MNTR_FRMT_DECIMAL7,
    CDDP_MNTR_FRMT_DECIMAL8,

    CDDP_MNTR_FRMT_SLIDER,
    CDDP_MNTR_FRMT_CHECKBOX,

    CDDP_MNTR_FRMT_LAST,
    CDDP_MNTR_FRMT_COUNT = CDDP_MNTR_FRMT_LAST - CDDP_MNTR_FRMT_FIRST
} cddp_mntr_frmt_t;


typedef uint64_t cddp_data_tick_t;

// Sensors for logging and display
typedef enum 
{
    CDDP_SNSR_FIRST = 0,

    CDDP_SNSR_TEST,

    CDDP_SNSR_LAST,
    CDDP_SNSR_COUNT = CDDP_SNSR_LAST - CDDP_SNSR_FIRST,
} cddp_snsr_t;


typedef struct 
{
    int ( *connect ) ( void );                  // open packet stream connection
    int ( *read    ) ( void*, size_t, size_t* );// read data update
    int ( *send    ) ( void*, size_t );         // send data update
    uint64_t ( *tick ) ( void );                // get current tick
    int ( *start ) ( void* ( *f ) ( void* ) );  // start the cddp processing task
    int ( *stop  ) ( void );                    // join with the cddp task

    bool ( *initialized  ) ( void );            // get initialization status
    bool ( *connected    ) ( void );            // get connected status
    bool ( *started      ) ( void );            // get task started status

} cddp_cfg_t; // cddp interface


typedef struct 
{
    bool             enabled;
    cddp_data_id_t   id;
    cddp_data_tick_t tick;
    uint8_t data[ CDDP_DATA_SIZE - sizeof( cddp_data_id_t ) - sizeof( cddp_data_tick_t ) ];
} cddp_data_buf_t;
// assert size == CDDP_DATA_SIZE ? - maybe want to store extra data on data other the data synced w/ other device


// public interface

int cddp_init( cddp_cfg_t* cfg, cddp_data_buf_t* buf, size_t buf_size );
int cddp_start( void );
int cddp_stop( void );

int cddp_data_enable( cddp_data_id_t id );
int cddp_data_disable( cddp_data_id_t id );

int cddp_data_set( cddp_data_id_t id, void* data );
int cddp_data_get( cddp_data_id_t id, void* data, cddp_data_tick_t* tick );

#endif /* _CDDP_PUBLIC_H_ */