#ifndef _PROX_PUBLIC_H_
#define _PROX_PUBLIC_H_

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// CDDP constants

#define CDDP_DATA_SIZE  128

// Prox types

typedef struct {

    int ( *get ) ( void* );                    // get most recent proximity sensory data
    int ( *start ) ( void* ( *f ) ( void* ) ); // enable the prox monitoring in the snsr lib
    int ( *stop  ) ( void );                   // disable the prox monitoring

} prox_cfg_t; // cddp interface


// public interface

int prox_init( void );
int prox_start( void );
int prox_stop( void );

void prox_data_get( void* data );

#endif /* _PROX_PUBLIC_H_ */