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


int main(int argc, char const *argv[]) 
{ 
	// local variables
	int 			 rc;
	cddp_data_id_t   id;
	cddp_data_tick_t tick;
	uint8_t 		 buf[ CDDP_DATA_SIZE ];

	struct timespec req;

	// init variables

	rc 		    = 1;
	req.tv_sec  = 1;
	req.tv_nsec = 150000000;

	id = 102;
	for( size_t i = 0; i < CDDP_DATA_SIZE; i++ )
	{
		buf[ i ] = i;
	}

	// cddp client examples
	cddp_init();
	cddp_data_on( 100 );
	
	size_t i = 0;
	while( rc )
	{
		cddp_data_set( id, buf ); 
		nanosleep( &req, &req );

		i++;
		if( i % 10 == 0 )
			printf("Looped: %ld\n", i);
	}

	return 0;
} 
