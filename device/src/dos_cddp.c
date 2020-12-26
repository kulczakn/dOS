#include "dos_cddp_public.h"

#include "sim_cddp_public.h"

#include "cddp_public.h"

// project side variables

cddp_cfg_t      cddp_cfg;                               // pointers to low level module functions
cddp_data_buf_t cddp_data_buf[ CDDP_DATA_ID_COUNT ];    // cddp data buffer

/**
 * @brief   dos_cddp_init initializes the cddp project side module by calling the hardware or simulated cddp interface indicated by the precompiler flags
 */
int dos_cddp_init( void )
{
    // local variables
    int rc = 1;

    // init low level driver

    #ifdef _CMAKE_SIM_BUILD_
    // init simulator driver

    sim_cddp_init( &cddp_cfg );

    #else 
    // init esp32 hardware driver

    #endif

    if( cddp_cfg.initialized() )
    {
        // if low level module was initialized, initialize high level module
    
        if( cddp_init( &cddp_cfg, &cddp_data_buf[ 0 ], sizeof( cddp_data_buf ) ) )
        {
            // if module succesfully initializes
            
            rc = 1;
        }
    }

    return rc;
}