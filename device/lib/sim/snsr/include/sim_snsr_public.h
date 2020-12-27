#ifndef _SIM_SNSR_PUBLIC_H_
#define _SIM_SNSR_PUBLIC_H_

#include <stdio.h>
#include <pthread.h>
#include <string.h>

#include "cddp_public.h"
#include "snsr_public.h"

// constants

#define SIM_SNSR_STACK_SIZE 4096

// types

enum 
{
    SIM_SNSR_DIST_ID_FIRST = SNSR_SIM_ID_FIRST + 10,
    SIM_SNSR_DIST_ID_LAST  = SIM_SNSR_DIST_ID_FIRST + 5
};

// public interface

int         sim_snsr_init( void );

snsr_tick_t sim_snsr_tick( void );
bool        sim_snsr_initialized( void );
bool        sim_snsr_started( void );

#endif /* _SIM_SNSR_PUBLIC_H_ */ 