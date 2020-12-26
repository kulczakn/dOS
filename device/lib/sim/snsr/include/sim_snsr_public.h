#ifndef _SIM_SNSR_PUBLIC_H_
#define _SIM_SNSR_PUBLIC_H_

#include <stdio.h>
#include <pthread.h>
#include <string.h>

#include "snsr_public.h"

// constants

#define SIM_SNSR_STACK_SIZE 4096

// public interface

int         sim_snsr_init( void );

snsr_tick_t sim_snsr_tick( void );
bool        sim_snsr_initialized( void );
bool        sim_snsr_started( void );

#endif /* _SIM_SNSR_PUBLIC_H_ */ 