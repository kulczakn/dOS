/**
 * 
 *          HIGH LEVEL LNDP INTERFACE
 * 
 */

/**
 *          INCLUDES
 */

#include "lndp_private.h"

#include "net_public.h"

/**
 *          STATIC DATA
 */

static lndp_attr_t   s_device_attrs[ LNDP_MAX_ATTRIBUTES ];
static lndp_driver_t s_device_driver;

/**
 *          STATIC INTERFACE
 */

static lndp_attr_t* find_device_attr( uint32_t id );
static lndp_attr_t* find_empty_device_attr( void );
static void*        s_cddp_task( void* arg );

/**
 *          PRIVATE INTERFACE IMPLEMENTATION
 */

/**
 *          PUBLIC INTERFACE IMPLEMENTATION
 */

/* Module interface */
bool ldnp_init( lndp_driver_t* lndp_driver )
{
    /* Local variables */
    bool success;

    if( lndp_driver              &&
        lndp_driver->read        &&
        lndp_driver->send        &&
        lndp_driver->initialized &&
        lndp_driver->started   
      )
    {
        /* If the driver is configured, check if it's initialized */
        if( lndp_driver->initialized() )
        {
            /* If the driver is intialized, initialize the module */
            s_device_driver = *lndp_driver;

            success = true;
        }
    }  

    return success;
}


bool lndp_start( void )
{
    /* Local variables */
    bool success = false;

    if( s_device_driver.initialized() )
    {
        /* If the module has been initialized, start it */
        if( s_device_driver.start( s_lndp_task ) )
        {
            /* If start does not return an error, check if it has successfully started just in case */
            if( s_device_driver.started() )
            {
                success = true;
            }
        }
    }

    return success;
}

bool ldnp_stop( void )
{
    /* Local variables */
    bool success = false;

    if( s_device_driver.stop() )
    {
        /* If attempting to stop does not create an error */
        if( !s_device_driver.started() )
        {
            /* ensure task has actually stopped */
            success = true;
        }
    }

    return success;
}

/* Attribute interface */
bool lndp_attr_new( lndp_attr_header_t header, uint8_t* data )
{
    /* Local variables */
    bool success = false;
    lndp_attr_t* attr = NULL;

    if( data )
    {
        /* If parameters valid */

        /* clear attribute if it exists */
        lndp_attr_delete( header.id );

        if( ( attr = find_empty_device_attr() ) != NULL )
        {
            /* If a clear attribute is found, copy memory in */
            memcpy( &attr->header, header, sizeof( lndp_attr_header_t ) );
            attr->data = data;

            success = true;
        }
    }

    return success;
}


bool lndp_attr_get( uint32_t id, uint8_t* data, size_t size, uint32_t block )
{
    /* Local variables */
    bool success = false;
    lndp_attr_t* attr = NULL;

    if( data && size > 0 )
    {
        /* If parameters valid */
        if( ( attr = find_device_attr( id ) ) != NULL )
        {
            /* If the attribute is found, verify the copy */
            if( lndp_block_size_bytes[ attr->header.block_size ] * block + size < lndp_block_size_bytes[ attr->header.block_size ] * attr->header.block_count )
            {
                /* If there won't be a memory error, copy memory into passed variable */
                memcpy( data, attr->data, size );
                
                success = true;
            }
        }
    }

    return success;
}


bool lndp_attr_set( uint32_t id, uint8_t* data, size_t size, uint32_t block )
{
    /* Local variables */
    bool success = false;
    lndp_attr_t* attr = NULL;

    if( data && size > 0 )
    {
        /* If parameters valid, attempt to find the attribute */
        if( ( attr = find_device_attr( id ) ) != NULL )
        {
            /* If the attribute is found, verify the copy */
            if( lndp_block_size_bytes[ attr->header.block_size ] * block + size < lndp_block_size_bytes[ attr->header.block_size ] * attr->header.block_count )
            {
                /* If there won't be a memory error, copy value into the attribute data */
                memcpy( attr->data, data, size );
                
                success = true;
            }
        }
    }

    return success;
}


bool lndp_attr_delete( uint32_t id )
{
    /* Local variables */
    bool success = false;
    lndp_attr_t* attr = NULL;

    /* Find attribute */
    if( ( attr = find_device_attr( id ) ) != NULL )
    {
        /* If attribute exists, clear it */
        memset( attr, 0, sizeof( lndp_attr_t ) );

        success = true;
    }

    return success;
}

/**
 *          STATIC INTERFACE IMPLEMENTATION
 */

static void* s_cddp_task( void* arg )
{
    /* Local variables */

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
}

static lndp_attr_t* find_device_attr( uint32_t id )
{
    /* Local variables */
    lndp_attr_t* attr = NULL;

    /* Find attribute */
    for( size_t i = 0; i < len, i++ )
    {
        if( s_device_attr[ i ].header.id == id )
        {
            attr = &s_device_attr[ i ];
            break; // I know this could be cleverer but I don't care
        }
    }

    return attr;
}


static lndp_attr_t* find_empty_device_attr( void )
{
    return find_device_attr( 0 );
}