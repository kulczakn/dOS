#include "cddp_private.h"

// static

static void*            cddp_task( void* arg );
static cddp_data_tick_t cddp_get_tick();

// static uint8_t s_data_buf[ CDDP_DATA_ID_COUNT ][ CDDP_DATA_SIZE ];
static bool             s_data_on[ CDDP_DATA_ID_COUNT ];
static uint8_t          s_socket;
static cddp_data_tick_t s_tick;
static cddp_data_tick_t s_tick_init;

static pthread_attr_t   s_cddp_thread_attr;
static pthread_t        s_cddp_thread_id;

static struct {
    cddp_data_id_t id;
    cddp_data_tick_t tick;
    uint8_t data[ CDDP_DATA_SIZE - sizeof( cddp_data_id_t ) - sizeof( cddp_data_tick_t ) ];
} s_data_buf[ CDDP_DATA_ID_COUNT ];
// assert size == 128


// public interface implementation

void cddp_init()
{
    // clear static buffers
    memset( s_data_buf, 0, CDDP_DATA_ID_COUNT * CDDP_DATA_SIZE );
    memset( s_data_on, 0, sizeof(bool) );

    // clear static variables
    s_socket = 0;
    s_tick   = 0;
    s_tick_init = 0;
}

void cddp_start()
{
    // local variables
    int rc = 0;

    // start task

    rc = pthread_attr_init(&s_cddp_thread_attr);
    rc = pthread_attr_setstacksize(&s_cddp_thread_attr, CDDP_STACK_SIZE);

    rc = pthread_create(&s_cddp_thread_id, &s_cddp_thread_attr, &cddp_task, NULL);
}


void cddp_stop()
{
    int rc = 0;
    
    rc = pthread_join(s_cddp_thread_id, NULL);
}


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
    memcpy( &s_data_buf[ id ].data, data, CDDP_DATA_SIZE );
    s_data_buf[ id ].tick = cddp_get_tick();
    s_data_buf[ id ].id = id;
}


void cddp_data_get( cddp_data_id_t id, void* data, cddp_data_tick_t* tick )
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
    memcpy( data, &s_data_buf[ id ].data, CDDP_DATA_SIZE );
    memcpy( tick, &s_data_buf[ id ].tick, sizeof( cddp_data_tick_t ) );
}


// private interface implementation

// static functions

static cddp_data_tick_t cddp_get_tick()
{
    if( s_tick_init )
    {
        s_tick = (cddp_data_tick_t)time(NULL) - s_tick_init;
    }
    return s_tick;
}


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

    // set static variables
    s_tick_init = (cddp_data_tick_t)time(NULL);
}

static void* cddp_task( void* arg )
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

        // scan through each packet to see if it's activated
        for( size_t i = CDDP_PRJ_DATA_ID_FIRST; i < CDDP_PRJ_DATA_ID_LAST; i++ )
        {
            // send active packets
            if( s_data_on[ i ] )
            {
                send(s_socket, &s_data_buf[ i ], CDDP_DATA_SIZE, 0);
            }
        }
    }

}