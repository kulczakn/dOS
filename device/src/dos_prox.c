#include "dos_prox_public.h"

#include "sim_prox_public.h"

#include "prox_public.h"

void dos_prox_init( void )
{
    // local constants
    #define _SIM_BUILD_ 1

    // local variables

    // init low level driver

    #ifdef _SIM_BUILD_
    // init simulator driver

    sim_prox_init();

    #else 
    // init esp32 hardware driver

    #endif

    // clean up

    #undef _SIM_BUILD_
}