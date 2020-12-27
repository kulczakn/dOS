#ifndef _SIM_SNSR_DIST_PUBLIC_H_
#define _SIM_SNSR_DIST_PUBLIC_H_

#include <pthread.h>

#include "snsr_public.h"

#include "sim_snsr_public.h"

// constants

// types

enum 
{
    SIM_SNSR_DIST_ID = SIM_SNSR_DIST_ID_FIRST
};

// public interface

int sim_snsr_dist_init( snsr_cfg_t* snsr_cfg );

#endif /* _SIM_SNSR_DIST_PUBLIC_H_ */