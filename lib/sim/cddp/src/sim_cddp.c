#include "cddp_private.h"

// static variables

static uint8_t          s_conn;
static cddp_data_tick_t s_tick;
static cddp_data_tick_t s_tick_init;

static pthread_attr_t   s_cddp_thread_attr;
static pthread_t        s_cddp_thread_id;

// cddp project side variables

cddp_cfg_t   cddp_cfg;
bool         cddp_is_init;


static cddp_data_tick_t sim_cddp_tick()
{
    if( s_tick_init )
    {
        s_tick = (cddp_data_tick_t)time(NULL) - s_tick_init;
    }
    return s_tick;
}


static int sim_cddp_connect( void )
{
    // local variables
    struct sockaddr_in serv_addr = { 0 };

    // create and configure socket
	if( (s_conn = socket(AF_INET, SOCK_STREAM, 0) ) < 0) 
	{ 
        printf("Error creating socket\n");
		return; // TODO
	} 

	serv_addr.sin_family = AF_INET; 
	serv_addr.sin_port = htons( CDDP_SERV_PORT ); 
	
	// Convert IPv4 and IPv6 addresses from text to binary form 
	if(inet_pton(AF_INET, CDDP_SERV_ADDR, &serv_addr.sin_addr) <= 0) 
	{
        printf("Error converting address\n");
		return; // TODO 
	} 

	if( connect(s_conn, (struct sockaddr *)&serv_addr, sizeof(serv_addr) ) < 0) 
	{
        printf("Error connecting to server\n");
		return; // TODO
	} 

    // set static variables
    s_tick_init = (cddp_data_tick_t)time(NULL);
}

static int sim_cddp_send( void* data, size_t size )
{
    send(s_conn, data, size, 0);
    printf("")
}


static int sim_cddp_start ( void* ( *f ) ( void* ) )
{   
    // local variables
    int rc = 0;

    // start task

    rc = pthread_attr_init(&s_cddp_thread_attr);
    rc = pthread_attr_setstacksize(&s_cddp_thread_attr, CDDP_STACK_SIZE);

    rc = pthread_create(&s_cddp_thread_id, &s_cddp_thread_attr, f, NULL);

    return rc;
}


static int sim_cddp_stop()
{
    int rc = 0;
    
    rc = pthread_join(s_cddp_thread_id, NULL);

    return rc;
}


void sim_cddp_init()
{
    // clear static variables
    s_conn = 0;
    s_tick = 0;
    s_tick_init = 0;

    memset( &cddp_cfg, 0, sizeof( cddp_cfg_t ) );

    // configure cddp interface
    cddp_cfg.connect = &sim_cddp_connect;
    cddp_cfg.send    = &sim_cddp_send;
    cddp_cfg.tick    = &sim_cddp_tick;
    cddp_cfg.start   = &sim_cddp_start;
    cddp_cfg.stop    = &sim_cddp_stop;

    // flag simulated hardware interface as initialized
    cddp_is_init = true;
}
