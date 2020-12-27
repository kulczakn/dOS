#include "dos_snsr_public.h"

// #include "sim_snsr_public.h"

#include "snsr_public.h"

/**
 * @brief   dos_snsr_init 
 * 
 */
int dos_snsr_init( void )
{
    // local variables
    int rc = -1;

    // init low level driver

    #ifdef _CMAKE_SIM_BUILD_
    // init simulator driver

    // sim_snsr_dist_init();

    #else 
    // init esp32 hardware driver

    #endif

    rc = 1;

    return rc;
}