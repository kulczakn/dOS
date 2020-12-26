#ifndef _SIM_CDDP_PUBLIC_H_
#define _SIM_CDDP_PUBLIC_H_

#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <arpa/inet.h> 
#include <sys/socket.h> 

#include "cddp_public.h"

// CDDP constants

#define SIM_CDDP_SERV_PORT  8989
#define SIM_CDDP_SERV_ADDR  "127.0.0.1"
#define SIM_CDDP_STACK_SIZE 4096

// CDDP SIM types


// public interface

int sim_cddp_init( cddp_cfg_t* cddp_cfg );

#endif /* _SIM_CDDP_PUBLIC_H_ */