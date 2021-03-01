#ifndef _LNDP_PUBLIC_H_
#define _LNDP_PUBLIC_H_

/**
 *          INCLUDES
 */

#include "err_public.h"

/**
 *          CONSTANTS
 */

/**
 *          TYPES
 */

typedef struct
{
    bool ( *read        ) ( void*, size_t, size_t*, size_t );
    bool ( *send        ) ( void*, size_t );
    bool ( *start       ) ( void* ( *f ) ( void* ) );
    bool ( *initialized ) ( void ); 
    bool ( *started     ) ( void );

    /*
    TODO

    LNDP Task

    1. Wait for wifi access point and stations to start

    If no network information is saved or saved information fails
    2. Scan until station connects
        - Attempt handshake with station
        - If fail mark station for potential ban and restart loop
        - Attempt to connect to given network
        - If fail mark station for potential ban and restart loop
    
    4. Open UDP socket
    5. Send device attributes to network

    6. Handle incoming and outgoing packets
    
    */

} lndp_driver_t;


typedef uint8_t lndp_block_size_t;
enum
{
    LNDP_BLOCK_SIZE_4B,
    LNDP_BLOCK_SIZE_8B,
    LNDP_BLOCK_SIZE_512B,
    LNDP_BLOCK_SIZE_1KB,

    LNDP_BLOCK_SIZE_COUNT
};


size_t lndp_block_size_number[ LNDP_BLOCK_SIZE_COUNT ] =
{
    4, 8, 512, 1024
};


typedef struct
{
    bool writable;
    uint8_t block_size;
    uint32_t interval;
    uint32_t block_count;
    uint32_t id;
    uint64_t ttl;
    uint64_t addr;
    uint64_t timestamp;
    char[16] name;

} lndp_attr_header_t;


typedef struct
{
    lndp_attr_header_t header;
    union
    {
        // TODO: pointers to predefined types of data
        // maybe a pointer to a "custom" attribute type to help support project side attributes if needed?
        // space pointed to should be block_size * block_count bytes
        uint8_t* data;                      
    }
} lndp_attr_t;


typedef struct
{
    bool write;
    uint8_t block_size;
    uint32_t block_count;
    uint32_t id;
    uint64_t address;
} lndp_pkt_header_t;


typedef union
{
    lndp_pkt_header_t header;
    union
    {
        // TODO: Can these be the same as the attributes?
        uint8_t* data;   
    }
} ldnp_pkt_t;


/**
 *          PUBLIC INTERFACE
 */

/* Module interface */
dos_err_t ldnp_init( void );
dos_err_t lndp_start( void );
dos_err_t ldnp_stop( void );

/* Attribute interface */
dos_err_t lndp_attr_new( void );
dos_err_t lndp_attr_get( void );
dos_err_t lndp_attr_set( void );
dos_err_t lndp_attr_delete( void );


#endif /* _LNDP_PUBLIC_H_ */