/***********************************************
 * 
 * 	
 * 
 * 
 * 
 * 
 * ********************************************/


#include <string.h> 
#include <time.h>
#include <unistd.h>
#include <errno.h>

/***** Project Includes *****/

#include "cddp_public.h"
#include "snsr_public.h"

#include "dos_cddp_public.h"
#include "dos_snsr_public.h"

// constants

// main

/**
 * @brief 	Entry point for project side device logic
 * 
 * @param 	argc 
 * @param 	argv 
 * @return 	int 
 */
int main(int argc, char const *argv[]) 
{ 
	// local variables
	int 	  rc;
	uint8_t   id;
	uint64_t  tick_one;
	uint64_t  tick_two;
	uint8_t   buf_one[ CDDP_DATA_SIZE ];
	uint8_t   buf_two[ CDDP_DATA_SIZE ];

	struct timespec req;

	// init variables
	rc 		    = 1;
	req.tv_sec  = 1;
	req.tv_nsec = 150000000;

	id = 53;

	// initialize modules
	// maybe pass to high level init and have high level init call it w/ pointers so to only use 1 interface
	dos_cddp_init();

	// configure high level modules
	cddp_intrf_enable( id, CDDP_DATA_FMRT_INT, false, 1, sizeof( uint64_t ), "Test #1" );
	cddp_intrf_enable( id, CDDP_DATA_FMRT_INT, true, 1, sizeof( uint64_t ), "Test #2" );

	// start high level module processing
	cddp_start();
	
	// example
	size_t i = 0;
	while( rc )
	{
		// set data
		*buf_one = i % 13;
		cddp_data_set( id, buf_one );

		// sleep
		nanosleep( &req, &req );

		// read data
		cddp_data_get( id, buf_one, &tick_one );
		cddp_data_get( id+1, buf_two, &tick_two );

		i++;
		if( i % 3 == 0 )
		{
			printf("Data #%u: %lu at %lu\n", id, (uint64_t)*buf_one, tick_one );
			printf("Data #%u: %lu at %lu\n", id+1, (uint64_t)*buf_two, tick_two );
		}
	}

	// stop high level module processing
	cddp_stop();

	// exit
	return 0;
} 
