/**
 * 
 *  FILE:       snsr.c
 *  MODULE:     SNSR
 * 
 *  DESC:       Sensor...
 * 
 **/

#include "snsr_private.h"

// static variables

static snsr_cfg_t*       s_snsr_cfgs;
static size_t            s_snsr_cnt;
static snsr_buf_t*       s_snsr_buf;
static snsr_start_task_t s_start_task;  // start the sensor processing task
static snsr_stop_task_t  s_stop_task;   // stop the sensor processing task 

// static functions

static void*             s_snsr_task( void* );               


/**
 * @brief   snsr_init initializes the high level snsr module
 * 
 * @param snsr_cfgs low level sensor module interfaces
 * @param snsr_cnt  number of sensor interfaces being used
 * @param buf       pointer to data buffer for high level module to use
 * @param buf_size  size of data buffer in bytes
 * @return int 
 */
int snsr_init( snsr_cfg_t* snsr_cfgs, 
               snsr_start_task_t snsr_start_task,
               snsr_stop_task_t snsr_stop_task, 
               size_t snsr_cnt, 
               void* buf, 
               size_t buf_size 
             )
{
    // local variables
    int rc = -1;
    snsr_cfg_t* cfg = NULL;

    if( snsr_cfgs    &&
        snsr_cnt > 0 &&
        buf          &&
        buf_size == sizeof( snsr_buf_t )
                  * snsr_cnt
      )
    {
        // if the parameters are valid
        for( size_t i = 0; i < snsr_cnt; i ++)
        {
            cfg = &snsr_cfgs[ i ];

            // if any of the sensor drivers are improperly initialized, break

            if( cfg              &&
                cfg->initialized &&
                cfg->read        &&
                cfg->tick
            )
            {
                // if the low level interface has been configured and the data buffer is good, check if the low level module has been initialized

                if( cfg->initialized() )
                {
                    // if low level module initialized, continue

                    rc = 1;
                }
                else
                {
                    // break, no tolerance for now 
                    // TODO: better error handling

                    rc = -1;

                    break;
                }
                
            }
            else
            {
                // flag an error and break;
                rc = -1;

                break;
            }
        }

        if( rc > 0 )
        {
            // if no errors occured setting functions, copy buffer pointer and counts
            
            s_snsr_cfgs = snsr_cfgs;
            s_snsr_buf  = buf;
            s_snsr_cnt  = snsr_cnt;

            s_start_task = snsr_start_task;
            s_stop_task  = snsr_stop_task;
        }
    }

    // validate input
    
    return rc;
}


int snsr_start( void )
{
    // local variables
    int rc = -1;

    if( s_start_task( s_snsr_task ) )
    {
        // if start does not return an error
        
        rc = 1;
    }

    return rc;
}


int snsr_stop( void )
{
    // local variables
    int rc = -1;

    if( s_stop_task() )
    {
        // if the task successfully stops

        rc = 1;
    }

    return rc;
}


int snsr_enable( snsr_id_t id )
{
    // local variables
    int rc = -1;
    snsr_buf_t* buf = NULL;

    // validate input
    if( id >= SNSR_ID_FIRST && 
        id <  SNSR_ID_LAST
      )
    {
        for( size_t i = 0; i < s_snsr_cnt; i ++)
        {
            // find the sensor to enable

            if( s_snsr_buf[ i ].id == id )
            {
                // if this is the sensor to enable, enable it

                buf = &s_snsr_buf[ i ];

                if( !s_snsr_buf[ i ].enabled )
                {
                    s_snsr_buf[ i ].enabled = true;

                    rc = 1;
                }
            }
        }
    }

    return rc;
}


int snsr_disable( snsr_id_t id )
{
    // local variables
    int rc = -1;
    snsr_cfg_t* cfg = NULL;

    // validate input
    if( id >= SNSR_ID_FIRST && 
        id <  SNSR_ID_LAST
      )
    {
        for( size_t i = 0; i < s_snsr_cnt; i ++)
        {
            // find the sensor to disable

            if( s_snsr_buf[ i ].id == id )
            {
                // if this is the correct sensor, disable it

                cfg = &s_snsr_cfgs[ i ];

                if( s_snsr_buf[ id ].enabled )
                {
                    // disable data id
                    s_snsr_buf[ id ].enabled = false;

                    rc = 1;
                }

            }
        }
    }

    return rc;
}


int snsr_get( snsr_id_t id, void* data, snsr_tick_t* tick )
{
    // local variables
    int rc = -1;
    snsr_buf_t* buf = NULL;

    if( id >= SNSR_ID_FIRST      && 
        id <  SNSR_ID_LAST       &&
        data
      )
    {
        // if the input is valid, find the correct sensor data

        for( size_t i = 0; i < s_snsr_cnt; i ++)
        {
            if( s_snsr_buf[ i ].id == id )
            {
                // if this is the sensor, check if it's enables

                buf = &s_snsr_buf[ i ];

                if( buf->enabled )
                {
                    // if it's enabled, get the data

                    memcpy( data, buf->data, SNSR_DATA_SIZE );
                    memcpy( tick, &buf->tick, sizeof( snsr_tick_t ) );
                    
                    rc = 1;
                }

            }
        }
    }
    
    return rc;
}


/**
 * @brief   s_snsr_task is the module task that periodically fetches data from each enabled sensor.
 * 
 * @param arg 
 * @return void* 
 */
static void* s_snsr_task( void* arg )
{
    // local variables
    int         err = 0;
    snsr_buf_t* buf = NULL;
    snsr_cfg_t* cfg = NULL;

    // start main loop
    while( err != -1 )
    {
        // process sensors

        for( size_t i = 0; i < s_snsr_cnt; i++ )
        {
            // process each enabled sensor
            cfg = &s_snsr_cfgs[ i ];
            buf = &s_snsr_buf[ i ];

            if( cfg->initialized() &&
                buf->enabled
            )
            {
                // if the sensor low level module has been initialized and the high level sensor processing is enabled

                // get data from each active sensor
                if( cfg->read( buf->data, SNSR_DATA_SIZE, &buf->tick ) )
                {
                    // if the task successfully reads from the sensor
                    // TODO: validate data before putting it in buffer
                
                    err = 1;
                }
                else
                {
                    err = 2;
                }
                
            }
        }

        // handle error
        if( err == 2)
        {
            // failed to get data from sensor

            // nothing to do yet

            // clear error
            err = 0;
        }
        if( err == -1 )
        {
            // unrecoverable error, cannot clear
        } 
    }

    return NULL;
}