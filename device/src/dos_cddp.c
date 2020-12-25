#include "dos_cddp_public.h"

#include "sim_cddp_public.h"

#include "cddp_public.h"

/**
 * @brief   dos_cddp_init initializes the cddp project side module by calling the hardware or simulated cddp interface indicated by the precompiler flags
 */
void dos_cddp_init( void )
{
    // local variables

    // init low level driver

    #ifdef _CMAKE_SIM_BUILD_
    // init simulator driver

    sim_cddp_init();

    #else 
    // init esp32 hardware driver

    #endif
}