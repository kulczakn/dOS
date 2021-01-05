#ifndef _CDDP_PUBLIC_H_
#define _CDDP_PUBLIC_H_

#include <limits.h>
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

#define CDDP_DATA_SIZE     64
#define CDDP_PKT_SIZE      128

#define CDDP_CONN_PKT_NAME_BYTELEN   16
#define CDDP_CONN_PKT_INTRFC_BYTELEN 24
#define CDDP_CONN_TIMEOUT            10000
#define CDDP_CONNACK_TIMEOUT         10000
#define CDDP_TASK_TIMEOUT            100

#define CDDP_SYS_ADDR 1
#define CDDP_ALL_ADDR 0xFFFFFFFF

// CDDP types

typedef enum
{
    CDDP_ERR_FIRST = 0,

    CDDP_ERR_NONE  = CDDP_ERR_FIRST,
    CDDP_ERR_KICK,
    CDDP_ERR_READ_FAIL,
    CDDP_ERR_SEND_FAIL,

    CDDP_ERR_LAST,

    CDDP_ERR_COUNT = CDDP_ERR_LAST - CDDP_ERR_FIRST
} cddp_err_t;

typedef enum 
{

    CDDP_DATA_ID_FIRST = 0,

    // System data IDs are reserved IDs used to handle module configuration and connecting to the hub
    CDDP_SYS_DATA_ID_FIRST = CDDP_DATA_ID_FIRST,

    CDDP_SYS_DATA_CONN     = CDDP_SYS_DATA_ID_FIRST,
    CDDP_SYS_DATA_CONNACK,
    CDDP_SYS_DATA_KICK,
    CDDP_SYS_DATA_DISCONN,

    CDDP_SYS_DATA_ID_LAST  = 9,

    // Sensor IDs are used to store and send data to connected sensors by the sensor module
    CDDP_SNSR_ID_FIRST     = 10,

    CDDP_SNSR_DIST_ID      = CDDP_SNSR_ID_FIRST,

    CDDP_SNSR_ID_LAST      = 19,

    CDDP_RES_ID_FIRST      = 92,    // 99 through 99 are reserved because only 192 bits are available to send the interface mask
    CDDP_RES_ID_LAST       = 99,    // so the simulated IDs would not be able to replicate some embedded IDs

    // Simulated Data IDs are used to simulate embedded functions, they are the embedded ID counterpart offset by 100
    CDDP_SIM_DATA_ID_FIRST = 100,


    CDDP_SIM_SNSR_ID_FIRST = CDDP_SIM_DATA_ID_FIRST,

    CDDP_SIM_SNSR_DIST_ID  = 110,


    CDDP_SIM_DATA_ID_LAST  = 192,


    CDDP_DATA_ID_LAST      = CDDP_SIM_DATA_ID_LAST,

    CDDP_SYS_DATA_ID_COUNT = CDDP_SYS_DATA_ID_LAST - CDDP_SYS_DATA_ID_FIRST + 1,
    CDDP_SIM_DATA_ID_COUNT = CDDP_SIM_DATA_ID_LAST - CDDP_SIM_DATA_ID_FIRST + 1,
    CDDP_DATA_ID_COUNT = CDDP_DATA_ID_LAST - CDDP_DATA_ID_FIRST

} cddp_data_id_t;


typedef enum {

    CDDP_DEVICE_FIRST = 0,

    CDDP_ESP32_DEVICE_DOS,
    CDDP_SIM_DEVICE_DOS = 101,

    CDDP_DEVICE_LAST = 255,

    CDDP_DEVICE_COUNT = CDDP_DEVICE_LAST - CDDP_DEVICE_FIRST + 1

} cddp_device_t;


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
    int ( *read    ) ( void*, size_t, size_t*, size_t );
                                                // read data update
    int ( *send    ) ( void*, size_t );         // send data update
    uint64_t ( *tick ) ( void );                // get current tick
    int ( *start ) ( void* ( *f ) ( void* ) );  // start the cddp processing task
    int ( *stop  ) ( void );                    // join with the cddp task

    bool ( *initialized  ) ( void );            // get initialization status
    bool ( *connected    ) ( void );            // get connected status
    bool ( *started      ) ( void );            // get task started status

} cddp_cfg_t; // cddp interface


typedef struct {
    uint8_t device;
    uint8_t intrf[ CDDP_CONN_PKT_INTRFC_BYTELEN ];
                                                // bitmask of enabled data ids
    uint8_t wrtbl[ CDDP_CONN_PKT_INTRFC_BYTELEN ];
                                                // if each data id is writeable

    uint8_t buf[ CDDP_DATA_SIZE - sizeof( uint8_t ) - CDDP_CONN_PKT_INTRFC_BYTELEN - CDDP_CONN_PKT_INTRFC_BYTELEN ];
} cddp_conn_data_t;


typedef struct
{
    uint32_t addr;

    uint8_t buf[ CDDP_DATA_SIZE - sizeof( uint32_t ) ];
} cddp_connack_data_t;



typedef struct 
{
    uint32_t         addr;
    cddp_data_id_t   id;
    cddp_data_tick_t tick;
    uint8_t data[ CDDP_DATA_SIZE ];
    uint8_t buf[ CDDP_PKT_SIZE -  CDDP_DATA_SIZE - sizeof( uint32_t ) - sizeof( cddp_data_id_t ) - sizeof( cddp_data_tick_t ) ];
} cddp_pkt_t;
// assert size == 128


typedef struct 
{
    bool       enabled;
    bool       writable;
    cddp_pkt_t pkt;
} cddp_intrf_t;
// assert size == 128 ? - maybe want to store extra data on data other the data synced w/ other device


// public interface

int cddp_init( cddp_cfg_t* cfg, cddp_intrf_t* buf, size_t buf_size );
int cddp_start( void );
int cddp_stop( void );

int cddp_data_enable( cddp_data_id_t id );
int cddp_data_disable( cddp_data_id_t id );

int cddp_data_set( cddp_data_id_t id, void* data );
int cddp_data_get( cddp_data_id_t id, void* data, cddp_data_tick_t* tick );

bool cddp_connected( void );

#endif /* _CDDP_PUBLIC_H_ */