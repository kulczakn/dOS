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
#define CDDP_TASK_TICKRATE           100

#define CDDP_SYS_ADDR 1
#define CDDP_ALL_ADDR 0xFFFFFFFF

// CDDP types

typedef uint8_t cddp_err_t;
enum {
    CDDP_ERR_FIRST = 0,

    CDDP_ERR_NONE  = CDDP_ERR_FIRST,
    CDDP_ERR_KICK,
    CDDP_ERR_READ_FAIL,
    CDDP_ERR_SEND_FAIL,

    CDDP_ERR_LAST,

    CDDP_ERR_COUNT = CDDP_ERR_LAST - CDDP_ERR_FIRST
};


typedef uint8_t cddp_data_id_t;
enum 
{
    CDDP_DATA_ID_FIRST = 0,

    // System data IDs are reserved IDs used to handle module configuration and connecting to the hub
    CDDP_SYS_DATA_ID_FIRST = CDDP_DATA_ID_FIRST,

    CDDP_SYS_DATA_CONN     = CDDP_SYS_DATA_ID_FIRST,
    CDDP_SYS_DATA_CONNACK,
    CDDP_SYS_DATA_INTRF,
    CDDP_SYS_DATA_KICK,
    CDDP_SYS_DATA_DISCONN,

    CDDP_SYS_DATA_ID_LAST  = 9,
    
    CDDP_RES_ID_FIRST      = 100,   // 0-9 are reserved for sys IDs, which the simulator also uses
    CDDP_RES_ID_LAST       = 109,

    // Simulated Data IDs are used to simulate embedded functions, they are the embedded ID counterpart offset by 100
    CDDP_SIM_DATA_ID_FIRST = 110,
    CDDP_SIM_DATA_ID_LAST  = 200,


    CDDP_DATA_ID_LAST      = CDDP_SIM_DATA_ID_LAST,

    CDDP_SYS_DATA_ID_COUNT = CDDP_SYS_DATA_ID_LAST - CDDP_SYS_DATA_ID_FIRST + 1,
    CDDP_SIM_DATA_ID_COUNT = CDDP_SIM_DATA_ID_LAST - CDDP_SIM_DATA_ID_FIRST + 1,
    CDDP_DATA_ID_COUNT     = CDDP_DATA_ID_LAST - CDDP_DATA_ID_FIRST
};


typedef uint8_t cddp_device_t;
enum {

    CDDP_DEVICE_FIRST = 0,

    CDDP_ESP32_DEVICE_DOS,
    CDDP_SIM_DEVICE_DOS = 101,

    CDDP_DEVICE_LAST = 255,

    CDDP_DEVICE_COUNT = CDDP_DEVICE_LAST - CDDP_DEVICE_FIRST + 1

};


// displayable data formats
typedef uint8_t cddp_data_frmt_t;
enum {
    CDDP_DATA_FRMT_FIRST = 0,

    CDDP_DATA_FMRT_UINT = CDDP_DATA_FRMT_FIRST,
    CDDP_DATA_FMRT_INT,
    CDDP_DATA_FRMT_STRING,
    CDDP_DATA_FRMT_FLOAT,
    CDDP_DATA_FRMT_DOUBLE,
    CDDP_DATA_FRMT_BOOL,

    CDDP_DATA_FRMT_LAST,
    CDDP_DATA_FRMT_COUNT = CDDP_DATA_FRMT_LAST - CDDP_DATA_FRMT_FIRST
};


typedef uint8_t cddp_handshake_step_t;
enum {
    CDDP_HANDSHAKE_FIRST = 0,

    CDDP_HANDSHAKE_INIT = CDDP_HANDSHAKE_FIRST,
    CDDP_HANDSHAKE_START,
    CDDP_HANDSHAKE_CONN_SENT,
    CDDP_HANDSHAKE_CONNACKED,
    CDDP_HANDSHAKE_INTRF_SENT,
    CDDP_HANDSHAKE_INTRF_CONNACKED,
    CDDP_HANDSHAKE_DONE,

    CDDP_HANDSHAKE_LAST = CDDP_HANDSHAKE_DONE,
    CDDP_HANDSHAKE_COUNT = CDDP_HANDSHAKE_LAST - CDDP_HANDSHAKE_FIRST
};


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


typedef union 
{
    struct 
    {
        uint8_t device;
        uint8_t intrf_count;
    };

    uint8_t buf[ CDDP_DATA_SIZE ];
} cddp_conn_data_t;


typedef union 
{
    struct 
    {
        uint8_t  id;                        /* Data ID                          */
        uint8_t  frmt;                      /* Data format                      */
        bool     wrtbl;                     /* If data is writable by server    */
        bool     immed;                     /* If data updates immediately send */ 
                                            /* update                           */
        uint32_t count;                     /* Numer of data blocks             */
        uint32_t size;                      /* Size of data                     */
        uint64_t intrvl;                    /* Ticks per update                 */
        char     name[16];                  /* Data name                        */
    };

    uint8_t buf[ CDDP_DATA_SIZE ];
} cddp_intrf_data_t;


typedef union
{
    struct
    {
        uint32_t addr;                              // assign device an address
        uint8_t  intrf_cnt;                         // confirm number of interfaces
    };

    uint8_t buf[ CDDP_DATA_SIZE ];
} cddp_connack_data_t;


typedef union 
{
    struct
    {
        uint32_t addr;
        uint8_t  id;
        uint64_t tick;
        uint32_t seq;
        union
        {
            cddp_conn_data_t    conn_data;
            cddp_intrf_data_t   intrf_data;
            cddp_connack_data_t connack_data;
            uint8_t             data[ CDDP_DATA_SIZE ];
        };  // TODO: Find a better way to do this
    };

    uint8_t  buf[ CDDP_PKT_SIZE ];
} cddp_pkt_t;
// assert size == 128


typedef struct 
{
    uint64_t tick;
    bool     enabled;
    union
    {
        cddp_conn_data_t    conn_data;
        cddp_intrf_data_t   intrf_data;
        cddp_connack_data_t connack_data;
        uint8_t             data[ CDDP_DATA_SIZE ];
    };      // TODO: So I don't have to do it twice

    cddp_intrf_data_t intrf;
} cddp_intrf_t;
// assert size == 128 ? - maybe want to store extra data on data other the data synced w/ other device


// public interface

int cddp_init( cddp_cfg_t* cfg, cddp_intrf_t* buf, size_t buf_size );
int cddp_start( void );
int cddp_stop( void );

int cddp_intrf_enable( uint8_t id, uint8_t frmt, bool wrtbl, uint32_t count, uint32_t size, const char* name );
int cddp_intrf_disable( uint8_t id );

int cddp_data_set( cddp_data_id_t id, void* data );
int cddp_data_get( cddp_data_id_t id, void* data, uint64_t* tick );

bool cddp_connected( void );

cddp_handshake_step_t cddp_get_handshake_step( void );
uint32_t cddp_get_addr( void );
uint8_t cddp_get_device( void );


#endif /* _CDDP_PUBLIC_H_ */