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


/**
 * @brief 
 * 
 * @param snsr_cfgs 
 * @param snsr_cnt 
 * @param buf 
 * @param buf_size 
 * @return int 
 */
int snsr_init( snsr_cfg_t* snsr_cfgs, size_t snsr_cnt, snsr_data_buf_t* buf, size_t buf_size )
{

}


int snsr_start( void )
{

}


int snsr_stop( void )
{

}


int snsr_enable( snsr_id_t id )
{

}


int snsr_disable( snsr_id_t id )
{

}


int snsr_get( snsr_id_t id, void* data, snsr_tick_t* tick )
{

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
    int err = 0;

    // connect to server
    if( true )
    {

        // if connect does not return error code
        if( true )
        {
            // if it actually connected

            // enter main loop
            while( err != -1 )
            {
                // read data from each sensor into local buffer               

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