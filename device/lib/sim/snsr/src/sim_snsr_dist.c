/**
 * 
 *  FILE:       sim_snsr_dist.c
 *  MODULE:     sim_SNSR
 * 
 *  DESC:       Simulated snsr interface using CDDP packets
 * 
 **/

#include "sim_snsr_dist_private.h"
#include "sim_snsr_private.h"

// static variables


static int sim_snsr_dist_read( void* data, size_t size, snsr_tick_t* tick )
{
    // local variables
    int rc             = -1;

    if( sim_snsr_initialized() &&
        sim_snsr_started()
      )
    {
        // if initialized and started, check if cddp is connected

        if( cddp_connected() )
        {
            // if cddp is connected, get the sim packet

            if( cddp_data_get( 110, data, tick ) )
            {
                // if we successfully read the packet

                rc = 1;
            }
        }
    }

    // if initialized and connected, read from the cddp sim data id to get the simulated data id and return it
    rc = 1;

    return rc;
}


/**
 *  @brief  sim_cddp_init initializes the data for the the simulated cddp module.
 *          This function will clear the modules static variables and will set the module interface functions in the cddp_cfg object.
 *          After this function has been executed, the module task can be started.
 * 
 *  @param  snsr_cfg    snsr functions
 **/
int sim_snsr_dist_init( snsr_cfg_t* snsr_cfg, snsr_buf_t* snsr_buf )
{
    // local variables
    int rc = -1;

    printf("\nSim CDDP initializing...\n");

    // clear cfg variable

    memset( snsr_cfg, 0, sizeof( snsr_cfg_t ) );
    memset( snsr_buf, 0, sizeof( snsr_buf_t ) );

    // configure cddp interface
    snsr_cfg->read = &sim_snsr_dist_read;
    snsr_cfg->tick = &sim_snsr_tick;            // helper?

    // configure cddp helper functions
    snsr_cfg->initialized = &sim_snsr_initialized;

    // flag simulated hardware interface as initialized
    if( snsr_cfg->initialized() )
    {
        // if module has initialized, init buffer

        snsr_buf->id = SNSR_SIM_DIST_ID;

        printf("Sim CDDP initialized.\n");

        rc = 1;
    }
   
    return rc;
}
