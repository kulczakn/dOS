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

#define CDDP_SIM_SERV_PORT  8989
#define CDDP_SIM_SERV_ADDR  "127.0.0.1"
#define CDDP_SIM_STACK_SIZE 4096

// CDDP SIM types


// public interface

void sim_cddp_init();

#endif /* _SIM_CDDP_PUBLIC_H_ */