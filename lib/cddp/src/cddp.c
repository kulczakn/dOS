/**
 * 
 *      HIGH LEVEL CDDP INTERFACE
 * 
 */

#include "cddp_private.h"

// static

static void*            cddp_task( void* arg );


// public interface implementation

int cddp_init( void )
{
    if( !cddp_is_init )
    {
        return -1; // TODO
    }

    // clear flags
    
    return 1;
}


int cddp_stop( void )
{
    return cddp_cfg.stop();
}


void cddp_data_enable( cddp_data_id_t id )
{
    // validate input
    if( id < CDDP_PRJ_DATA_ID_FIRST || id > CDDP_PRJ_DATA_ID_LAST )
    {
        return; // TODO
    }

    if( cddp_cfg.data_en[ id ] )
    {
        return; // TODO
    }

    // turn data on
    cddp_cfg.data_en[ id ] = true;

}


void cddp_data_disable( cddp_data_id_t id )
{
    // validate input
    if( id < CDDP_PRJ_DATA_ID_FIRST || id > CDDP_PRJ_DATA_ID_LAST )
    {
        return; // TODO
    }

    if( !cddp_cfg.data_en[ id ] )
    {
        return; // TODO
    }

    // turn data on
    cddp_cfg.data_en[ id ] = false;

}

void cddp_data_set( cddp_data_id_t id, void* data )
{
    // validate input
    if( id < CDDP_PRJ_DATA_ID_FIRST || id > CDDP_PRJ_DATA_ID_LAST )
    {
        return; // TODO
    }

    if( !cddp_cfg.data_en[ id ] )
    {
        return; // TODO
    }

    if( !data )
    {
        return; // TODO
    }

    // copy data over
    memcpy( &cddp_cfg.data_buf[ id ].data, data, CDDP_DATA_SIZE );
    cddp_cfg.data_buf[ id ].tick = cddp_cfg.tick();
    cddp_cfg.data_buf[ id ].id = id;
}


void cddp_data_get( cddp_data_id_t id, void* data, cddp_data_tick_t* tick )
{
    // validate input
    if( id < CDDP_PRJ_DATA_ID_FIRST || id > CDDP_PRJ_DATA_ID_LAST )
    {
        return; // TODO
    }

    if( !cddp_cfg.data_en[ id ] )
    {
        return; // TODO
    }

    if( !data )
    {
        return; // TODO
    }

    // copy data over
    memcpy( data, &cddp_cfg.data_buf[ id ].data, CDDP_DATA_SIZE );
    memcpy( tick, &cddp_cfg.data_buf[ id ].tick, sizeof( cddp_data_tick_t ) );
}


// private interface implementation

// static functions

static void* cddp_task( void* arg )
{
    // local variables

    // connect to server
    cddp_cfg.connect();

    // main loop
    while( true )
    {
        // read from socket

        // update data

        // write data to socket

        // scan through each packet to see if it's activated
        for( size_t i = CDDP_PRJ_DATA_ID_FIRST; i < CDDP_PRJ_DATA_ID_LAST; i++ )
        {
            // send active packets
            if( cddp_cfg.data_en[ i ] )
            {
                cddp_cfg.send( &cddp_cfg.data_buf[ i ], CDDP_DATA_SIZE );
            }
        }
    }

}