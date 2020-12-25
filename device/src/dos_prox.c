#include "dos_prox_public.h"

#include "sim_prox_public.h"

#include "prox_public.h"

/**
 * @brief   dos_prox_init initializes the prox project side module by calling the hardware or simulated prox interface indicated by the precompiler flags
 */
void dos_prox_init( void )
{
    // local variables

    // init low level driver

    #ifdef _CMAKE_SIM_BUILD_
    // init simulator driver

    sim_prox_init();

    #else 
    // init esp32 hardware driver

    #endif
}