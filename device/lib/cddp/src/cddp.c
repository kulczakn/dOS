/**
 * 
 *      HIGH LEVEL CDDP INTERFACE
 * 
 */

#include "cddp_private.h"

// static

static void*            cddp_task( void* arg );


cddp_data_buf_t cddp_data_buf[ CDDP_DATA_ID_COUNT ];     // buffered data for each data id
                                                    // MAJOR TODO: MOVE THIS MEMORY TO PROJECT SIDE


// public interface implementation

/**
 * @brief   cddp_init initializes the high level cddp module
 * 
 * @return int 
 */
int cddp_init( void )
{
    // local variables
    int rc = -1;

    if( !cddp_is_init )
    {
        // nothing much to do yet
        rc = 1;
    }
    
    return rc;
}


/**
 * @brief   cddp_start starts the cddp module task using the low level 
 * 
 * @return int 
 */
int cddp_start( void )
{
    return cddp_cfg.start( cddp_task );
}


/**
 * @brief   cddp_stop stops the cddp module task
 * 
 * @return int 
 */
int cddp_stop( void )
{
    return cddp_cfg.stop();
}


/**
 * @brief   cddp_data_enable enables a data id
 * 
 * @param   id  data id to enable
 */
void cddp_data_enable( cddp_data_id_t id )
{
    // local variables
    int rc = -1;

    // validate input
    if( id < CDDP_PRJ_DATA_ID_FIRST || 
        id > CDDP_PRJ_DATA_ID_LAST  ||
        cddp_data_buf[ id ].enabled
      )
    {
        rc = -1;
    }
    else
    {
        // enable data id
        cddp_data_buf[ id ].enabled = true;

        rc = 1;
    }

    // return rc;
}


/**
 * @brief   cddp_data_disable disables a data id
 * 
 * @param   id  data id to disable
 */
void cddp_data_disable( cddp_data_id_t id )
{
    // local variables
    int rc = -1;

    // validate input
    if( id < CDDP_PRJ_DATA_ID_FIRST || 
        id > CDDP_PRJ_DATA_ID_LAST  ||
        !cddp_data_buf[ id ].enabled
      )
    {
        rc = -1;
    }
    else
    {
        // disable data id
        cddp_data_buf[ id ].enabled = false;

        rc = 1;
    }

    // return rc;
}


/**
 * @brief   cddp_data_set updates a data id's data
 * 
 * @param   id      data id to update
 * @param   data    data to copy in
 */
void cddp_data_set( cddp_data_id_t id, void* data )
{
    // local variables
    int rc = -1;

    // validate input
    if( id < CDDP_PRJ_DATA_ID_FIRST || 
        id > CDDP_PRJ_DATA_ID_LAST  || 
        !data                       ||
        !cddp_data_buf[ id ].enabled
      )
    {
        // invalid data or 
        return; // TODO
    }
    else
    {
        // copy data over
        memcpy( &cddp_data_buf[ id ].data, data, CDDP_DATA_SIZE );

        // update 
        cddp_data_buf[ id ].tick = cddp_cfg.tick();
        cddp_data_buf[ id ].id = id;
    }
}


/**
 * @brief 
 * 
 * @param id 
 * @param data 
 * @param tick 
 */
void cddp_data_get( cddp_data_id_t id, void* data, cddp_data_tick_t* tick )
{
    // local variables
    int rc = -1;

    // validate input
    if( id < CDDP_PRJ_DATA_ID_FIRST  || 
        id > CDDP_PRJ_DATA_ID_LAST   ||
        !cddp_data_buf[ id ].enabled ||
        !data
      )
    {
        rc = -1;
    }
    else
    {
        // copy data over
        memcpy( data, &cddp_data_buf[ id ].data, CDDP_DATA_SIZE );
        memcpy( tick, &cddp_data_buf[ id ].tick, sizeof( cddp_data_tick_t ) );
        
        rc = 1;
    }

    // return rc;
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
            if( cddp_data_buf[ i ].enabled )
            {
                cddp_cfg.send( &cddp_data_buf[ i ] + sizeof(bool), CDDP_DATA_SIZE ); // TODO M U C H more elegant solution
            }
        }
    }

}