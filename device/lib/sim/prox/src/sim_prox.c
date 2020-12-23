#include "sim_prox_private.h"

// static variables

// prox project side variables

prox_cfg_t prox_cfg;
bool       prox_is_init;


static int sim_prox_get( void* data, uint64_t* tick )
{
    // copy proximity data from simulated cddp data to param
    cddp_data_get( 300/* sim prox */, data, tick );

    return 1;
}


static int sim_prox_start( void )
{
    return 1;
}


static int sim_prox_stop( void )
{
    return 1;
}


void sim_prox_init()
{
    // clear static variables
    memset( &prox_cfg, 0, sizeof(prox_cfg_t) );
    prox_is_init = false;

    // configure prox interface
    prox_cfg.get   = sim_prox_get;
    prox_cfg.start = sim_prox_start;
    prox_cfg.stop  = sim_prox_stop;

    // flag simulated hardware interface as initialized
    prox_is_init = true;
}
