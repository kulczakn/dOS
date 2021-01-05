#include "dos_cddp_public.h"

#include "sim_cddp_public.h"

#include "cddp_public.h"

// project side variables

cddp_cfg_t   cddp_cfg;                          // pointers to low level module functions
cddp_intrf_t cddp_intrf[ CDDP_DATA_ID_COUNT ];  // cddp data buffer

/**
 * @brief   dos_cddp_init initializes the cddp project side module by calling the hardware or simulated cddp interface indicated by the precompiler flags
 */
int dos_cddp_init( void )
{
    // local variables
    int     rc     = 1;
    uint8_t device = 0;

    cddp_conn_data_t device_conn_data = { 0 };

    // init low level driver

    #ifdef _CMAKE_SIM_BUILD_
    // init simulator driver

    sim_cddp_init( &cddp_cfg );
    device = CDDP_SIM_DEVICE_DOS;

    #else 
    // init esp32 hardware driver

    device = CDDP_ESP32_DEVICE_DOS;

    #endif

    if( cddp_cfg.initialized() )
    {
        // if low level module was initialized, initialize high level module
    
        if( cddp_init( &cddp_cfg, &cddp_intrf[ 0 ], sizeof( cddp_intrf ) ) )
        {
            // if module succesfully initializes, configure device data

            device_conn_data.device = device;
            
            rc = 1;
        }
    }

    return rc;
}