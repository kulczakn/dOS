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


static int sim_snsr_dist_read( void* data, size_t size )
{
    // local variables
    int rc = -1;

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
int sim_snsr_dist_init( snsr_cfg_t* snsr_cfg )
{
    // local variables
    int rc = -1;

    printf("\nSim CDDP initializing...\n");

    // initialize sim snsr module

    sim_snsr_init();

    memset( snsr_cfg, 0, sizeof( snsr_cfg_t ) );

    // configure cddp interface
    snsr_cfg->read = &sim_snsr_dist_read;
    snsr_cfg->tick = &sim_snsr_tick;            // helper?

    // configure cddp helper functions
    snsr_cfg->initialized = &sim_snsr_initialized;
    snsr_cfg->started     = &sim_snsr_started;

    // flag simulated hardware interface as initialized
    if( snsr_cfg->initialized() )
    {
        // if module has initialized, return 1
        printf("Sim CDDP initialized.\n");

        rc = 1;
    }
   
    return rc;
}
