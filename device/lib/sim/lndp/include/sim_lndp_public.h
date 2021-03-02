#ifndef _SIM_LNDP_PUBLIC_H_
#define _SIM_LNDP_PUBLIC_H_

#include <limits.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "lndp_public.h"

/**
 *          CONSTANTS
 */

#define SIM_LNDP_SERVER_PORT  8001
#define SIM_LNDP_STACK_SIZE   PTHREAD_STACK_MIN 

/**
 *          TYPES
 */

/**
 *          PUBLIC INTERFACE
 */

int sim_lndp_init( lndp_driver_t* lndp_driver );

#endif /* _SIM_LNDP_PUBLIC_H_ */