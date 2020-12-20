#include "cddp_private.h"

// static

static uint8_t s_data_buf[ CDDP_DATA_ID_COUNT ][ CDDP_DATA_SIZE ];
static bool    s_data_on[ CDDP_DATA_ID_COUNT ];
static uint8_t s_socket;


// public interface implementation

void cddp_init()
{
    // clear static buffers
    memset( s_data_buf, 0, CDDP_DATA_ID_COUNT * CDDP_DATA_SIZE );
    memset( s_data_on, 0, sizeof(bool) );

    // clear static variables
    s_socket = 0;

}

// void cddp_start();
// void cddp_stop());

void cddp_data_on( cddp_data_id_t id )
{
    // validate input
    if( id < CDDP_PRJ_DATA_ID_FIRST || id > CDDP_PRJ_DATA_ID_LAST )
    {
        return; // TODO
    }

    if( s_data_on[ id ] )
    {
        return; // TODO
    }

    // turn data on
    s_data_on[ id ] = true;

}


void cddp_data_off( cddp_data_id_t id )
{
    // validate input
    if( id < CDDP_PRJ_DATA_ID_FIRST || id > CDDP_PRJ_DATA_ID_LAST )
    {
        return; // TODO
    }

    if( !s_data_on[ id ] )
    {
        return; // TODO
    }

    // turn data on
    s_data_on[ id ] = false;

}

void cddp_data_set( cddp_data_id_t id, void* data )
{
    // validate input
    if( id < CDDP_PRJ_DATA_ID_FIRST || id > CDDP_PRJ_DATA_ID_LAST )
    {
        return; // TODO
    }

    if( !s_data_on[ id ] )
    {
        return; // TODO
    }

    if( !data )
    {
        return; // TODO
    }

    // copy data over
    memcpy( &s_data_buf[ id ], data, CDDP_DATA_SIZE );
}


void cddp_data_get( cddp_data_id_t id, void* data )
{
    // validate input
    if( id < CDDP_PRJ_DATA_ID_FIRST || id > CDDP_PRJ_DATA_ID_LAST )
    {
        return; // TODO
    }

    if( !s_data_on[ id ] )
    {
        return; // TODO
    }

    if( !data )
    {
        return; // TODO
    }

    // copy data over
    memcpy( data, &s_data_buf[ id ], CDDP_DATA_SIZE );
}


// private interface implementation

// static functions

static void cddp_connect( void )
{
    // local variables
    struct sockaddr_in serv_addr = { 0 };

    // create and configure socket
	if((s_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
	{ 
		return; // TODO
	} 

	serv_addr.sin_family = AF_INET; 
	serv_addr.sin_port = htons( CDDP_SERV_PORT ); 
	
	// Convert IPv4 and IPv6 addresses from text to binary form 
	if(inet_pton(AF_INET, CDDP_SERV_ADDR, &serv_addr.sin_addr) <= 0) 
	{
		return; // TODO 
	} 

	if(connect(s_socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
	{
		return; // TODO
	} 
}

static void cddp_task( void )
{
    // local variables

    // connect to server
    cddp_connect();

    // main loop
    while( true )
    {
        // read from socket

        // update data

        // write data to socket
    }

}