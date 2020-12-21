#include "dos_cddp_private.h"

#include "sim_cddp_public.h"

#include "cddp_public.h"

void dos_cddp_init( void )
{
    // local constants
    #define _SIM_BUILD_ 1

    // local variables

    // init low level driver

    #ifdef _SIM_BUILD_
    // init simulator driver

    sim_cddp_init();

    #else 
    // init esp32 hardware driver

    #endif

    // clean up

    #undef _SIM_BUILD_
}