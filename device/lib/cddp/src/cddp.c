/**
 * 
 *      HIGH LEVEL CDDP INTERFACE
 * 
 */

#include "cddp_private.h"

// static

static void*     s_cddp_task( void* arg );      // cddp module processing task
cddp_cfg_t       s_cddp_cfg;                    // pointers to cddp IO interface 
cddp_data_buf_t* s_cddp_data_buf;               // pointer to cddp data buffer in project side memory, MUST be size sizeof(cddp_data_buf_t) * CDDP_DATA_ID_COUNT

// public interface implementation

/**
 * @brief   cddp_init initializes the high level cddp module
 * 
 * @param   cfg         low level module function pointers
 * @param   buf         cddp data buffer in project side memory  
 * @param   buf_size    size of data buffer, must be sizeof(cddp_data_buf_t) * CDDP_DATA_ID_COUNT
 * 
 * @return int 
 */
int cddp_init( cddp_cfg_t* cddp_cfg, cddp_data_buf_t* cddp_buf, size_t buf_size )
{
    // local variables
    int rc = -1;

    // validate input
    if( cddp_cfg              &&
        cddp_cfg->connect     &&
        cddp_cfg->read        &&
        cddp_cfg->start       &&
        cddp_cfg->stop        &&
        cddp_cfg->send        &&
        cddp_cfg->connected   &&
        cddp_cfg->initialized &&
        cddp_cfg->started     &&
        cddp_cfg->tick        &&
        cddp_buf              &&
        buf_size == sizeof(cddp_data_buf_t) 
                  * CDDP_DATA_ID_COUNT
     )
    {
        // if the low level interface has been configured and the data buffer is good, check if the low level module has been initialized

        if( cddp_cfg->initialized() )
        {
            // if low level module initialized, copy data into static variables
            s_cddp_cfg = *cddp_cfg;
            s_cddp_data_buf = cddp_buf;

            rc = 1;
        }
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
    // local variables
    int rc = -1;

    if( s_cddp_cfg.initialized() )
    {
        //  if the module has been initialized, start it

        if( s_cddp_cfg.start( s_cddp_task ) > 0 )
        {
            // if start does not return an error, check if it has successfully started just in case

            if( s_cddp_cfg.started() > 0 )
            {
                rc = 1;
            }
        }
    }

    return rc;
}


/**
 * @brief   cddp_stop stops the cddp module task
 * 
 * @return int 
 */
int cddp_stop( void )
{
    // local variables
    int rc = -1;

    if( s_cddp_cfg.stop() )
    {
        // if attempting to stop does not create an error

        if( !s_cddp_cfg.started() )
        {
            // ensure task has actually stopped

            rc = 1;
        }
    }

    return rc;
}


/**
 * @brief   cddp_data_enable enables a data id
 * 
 * @param   id  data id to enable
 */
int cddp_data_enable( cddp_data_id_t id )
{
    // local variables
    int rc = -1;

    // validate input
    if( id >= CDDP_PRJ_DATA_ID_FIRST && 
        id <  CDDP_PRJ_DATA_ID_LAST  &&
        !s_cddp_data_buf[ id ].enabled
      )
    {
        s_cddp_data_buf[ id ].enabled = true;

        rc = 1;
    }

    return rc;
}


/**
 * @brief   cddp_data_disable disables a data id
 * 
 * @param   id  data id to disable
 */
int cddp_data_disable( cddp_data_id_t id )
{
    // local variables
    int rc = -1;

    // validate input
    if( id >= CDDP_PRJ_DATA_ID_FIRST && 
        id <  CDDP_PRJ_DATA_ID_LAST  &&
        s_cddp_data_buf[ id ].enabled
      )
    {
        // disable data id
        s_cddp_data_buf[ id ].enabled = false;

        rc = 1;
    }

    return rc;
}


/**
 * @brief   cddp_data_set updates a data id's data
 * 
 * @param   id      data id to update
 * @param   data    data to copy in
 */
int cddp_data_set( cddp_data_id_t id, void* data )
{
    // local variables
    int rc = -1;

    // validate input
    if( id >= CDDP_PRJ_DATA_ID_FIRST && 
        id <  CDDP_PRJ_DATA_ID_LAST  && 
        data                         &&
        s_cddp_data_buf[ id ].enabled
      )
    {
        // invalid data or 
        // copy data over
        memcpy( &s_cddp_data_buf[ id ].data, data, CDDP_DATA_SIZE );

        // update 
        s_cddp_data_buf[ id ].tick = s_cddp_cfg.tick();
        s_cddp_data_buf[ id ].id = id;

        rc = 1;
    }

    return rc;
}


/**
 * @brief 
 * 
 * @param id 
 * @param data 
 * @param tick 
 */
int cddp_data_get( cddp_data_id_t id, void* data, cddp_data_tick_t* tick )
{
    // local variables
    int rc = -1;

    // validate input
    if( id >= CDDP_PRJ_DATA_ID_FIRST  && 
        id <  CDDP_PRJ_DATA_ID_LAST   &&
        s_cddp_data_buf[ id ].enabled   &&
        data
      )
    {
        // copy data over
        memcpy( data, &s_cddp_data_buf[ id ].data, CDDP_DATA_SIZE );
        memcpy( tick, &s_cddp_data_buf[ id ].tick, sizeof( cddp_data_tick_t ) );
        
        rc = 1;
    }
    
    return rc;
}


// private interface implementation

// static functions

static void* s_cddp_task( void* arg )
{
    // local variables
    int err = 0;

    // connect to server
    if( s_cddp_cfg.connect() )
    {

        // if connect does not return error code
        if( s_cddp_cfg.connected() )
        {
            // if it actually connected

            // enter main loop
            while( err != -1 )
            {
                // read from socket

                // update data

                // write data to socket

                // scan through each packet to see if it's activated
                for( size_t i = CDDP_PRJ_DATA_ID_FIRST; i < CDDP_PRJ_DATA_ID_LAST; i++ )
                {
                    // send active packets
                    if( s_cddp_data_buf[ i ].enabled )
                    {
                        if( s_cddp_cfg.send( &s_cddp_data_buf[ i ] + sizeof(bool), CDDP_DATA_SIZE ) ) // TODO M U C H more elegant solution
                        {
                            // if send does not return error code
                            
                            // nothing to do
                        }
                        else
                        {
                            err = 2; // TODO: no more magic numbers
                        }
                    }
                }

                // handle errors
                if( err == 2)
                {
                    // if a data update failed to send

                    // nothing to do yet

                    // clear error
                    err = 0;
                }
                if( err = -1 )
                {
                    // unrecoverable error, cannot clear
                } 
            }
        }
    }
  
    
}