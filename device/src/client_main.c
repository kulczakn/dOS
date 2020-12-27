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
	int 			 rc;
	cddp_data_id_t   id;
	cddp_data_tick_t tick;
	uint8_t 		 buf[ SNSR_DATA_SIZE ];

	struct timespec req;

	// init variables
	rc 		    = 1;
	req.tv_sec  = 1;
	req.tv_nsec = 150000000;

	id = 210;

	// initialize modules
	// maybe pass to high level init and have high levl init call it w/ pointers so to only use 1 interface
	dos_cddp_init();
	dos_snsr_init(); 

	// configure high level modules
	cddp_data_enable( id );
	snsr_enable( id );
	// enable sensor

	// start high level module processing
	cddp_start();
	snsr_start();
	
	// example
	size_t i = 0;
	while( rc )
	{
		// cddp_data_set( id, buf ); 

		// read sensor data
		snsr_get( id, buf, &tick );

		nanosleep( &req, &req );

		i++;
		if( i % 3 == 0 )
			printf("Sensor: %d\n", (uint32_t)(*buf));
	}

	// stop high level module processing
	cddp_stop();
	snsr_stop();

	// exit
	return 0;
} 
