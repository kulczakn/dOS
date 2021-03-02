#ifndef _LNDP_PUBLIC_H_
#define _LNDP_PUBLIC_H_

/**
 *          INCLUDES
 */

// #include "err_public.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

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


size_t lndp_block_size_bytes[ LNDP_BLOCK_SIZE_COUNT ] =
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
    char     name[16];

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
    };
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
    };
} ldnp_pkt_t;


/**
 *          PUBLIC INTERFACE
 */

/* Module interface */
bool ldnp_init( lndp_driver_t* lndp_driver );
bool lndp_start( void );
bool ldnp_stop( void );

/* Attribute interface */
bool lndp_attr_new( lndp_attr_header_t header, uint8_t* data );
bool lndp_attr_get( uint32_t id, uint8_t* data, size_t size, uint32_t block );
bool lndp_attr_set( uint32_t id, uint8_t* data, size_t size, uint32_t block );
bool lndp_attr_delete( uint32_t id );


#endif /* _LNDP_PUBLIC_H_ */