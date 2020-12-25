#include "sim_cddp_private.h"

// static variables

static uint8_t          s_conn;
static cddp_data_tick_t s_tick;
static cddp_data_tick_t s_tick_init;
static bool             s_init;
static bool             s_connected;

static pthread_attr_t   s_cddp_thread_attr;
static pthread_t        s_cddp_thread_id;

// cddp project side variables

cddp_cfg_t   cddp_cfg;

/**
 *  @brief  If the tick has been initialized, sim_cddp_tick will update the update and return the current tick.
 *          Otherwise, it will return 0.
 *  
 **/
static cddp_data_tick_t sim_cddp_tick()
{
    // local variables
    cddp_data_tick_t ret = 0;

    if( s_tick_init )
    {
        // if tick is initialized, update tick
        s_tick = (cddp_data_tick_t)time(NULL) - s_tick_init;

        ret = s_tick;
    }

    // return current tick or 0
    return ret;
}


/**
 *  @brief  sim_cddp_initialized returns the status of the low level module initialization
 **/
static bool sim_cddp_initialized()
{
    return s_init;
}


/**
 *  @brief  sim_cddp_connected returns the low level module connected status
 **/
static bool sim_cddp_connected()
{
    return s_connected;
}


/**
 * @brief   sim_cddp_connect will attempt to connnect to the cddp socket interface.
 * 
 * @return  int
 */
static int sim_cddp_connect( void )
{
    printf("\nSim CDDP connecting...\n");

    // local variables
    struct sockaddr_in serv_addr = { 0 };

    int rc = -1;

    // create and configure socket
	if( (s_conn = socket(AF_INET, SOCK_STREAM, 0) ) < 0) 
	{ 
        printf("Error creating socket\n");
		rc = -1;
	}
    else
    {
        // if the socket was created succesfully

        serv_addr.sin_family = AF_INET; 
        serv_addr.sin_port = htons( SIM_CDDP_SERV_PORT ); 
        
        // Convert IPv4 and IPv6 addresses from text to binary form 
        if( inet_pton( AF_INET, SIM_CDDP_SERV_ADDR, &serv_addr.sin_addr ) <= 0 ) 
        {
            printf("Error converting address\n");
            rc = -1;
        }
        else
        {
            // if the address was able to be converted to a binary form

            if( connect( s_conn, (struct sockaddr *)&serv_addr, sizeof(serv_addr) ) < 0) 
            {
                printf("Error connecting to server\n");
                rc = -1;
            }
            else
            {
                // if the socket was able to connect

                // set static variables
                s_tick_init = (cddp_data_tick_t)time( NULL );

                printf("Sim CDDP connected.\n");

                rc = 1;
            }
        }
    }
    
    return rc;
}


/**
 * @brief   sim_cddp_send sends a data update across the socket interface
 * 
 * @param   data  data to be sent
 * @param   size  size of the data
 * @return int 
 */
static int sim_cddp_send( void* data, size_t size )
{
    send(s_conn, data, size, 0);
    // printf("")
    return 1;
}

/**
 *  @brief  sim_cddp_start starts the module task.
 *          After this function has been executed, the device code will be able to access the cddp data interface.
 *          
 *   @param f  Function that will be executed as the module task
 **/
static int sim_cddp_start ( void* ( *f ) ( void* ) )
{   
    printf("\nSim CDDP starting...\n");

    // local variables
    int rc = 0;

    if( !s_connected )
    {
        rc = -1;
    }
    else
    {
        // if connected, start task

        rc = pthread_attr_init( &s_cddp_thread_attr );
        rc = pthread_attr_setstacksize( &s_cddp_thread_attr, SIM_CDDP_STACK_SIZE );

        rc = pthread_create( &s_cddp_thread_id, &s_cddp_thread_attr, f, NULL );

        rc = 1;

        printf("Sim CDDP started.\n");
    }

    return rc;
}


/**
 *  @brief  sim_cddp_stop stops the module task.
 **/
static int sim_cddp_stop()
{
    // local variables
    int rc = 0;
    
    rc = pthread_join(s_cddp_thread_id, NULL);

    return rc;
}

/**
 *  @brief  sim_cddp_init initializes the data for the the simulated cddp module.
 *          This function will clear the modules static variables and will set the module interface functions in the cddp_cfg object.
 *          After this function has been executed, the module task can be started.
 **/
void sim_cddp_init()
{
    printf("\nSim CDDP initializing...\n");

    // clear static variables
    s_conn = 0;
    s_tick = 0;
    s_tick_init = 0;

    memset( &cddp_cfg, 0, sizeof( cddp_cfg_t ) );

    // configure cddp interface
    cddp_cfg.connect = &sim_cddp_connect;
    cddp_cfg.send    = &sim_cddp_send;
    cddp_cfg.tick    = &sim_cddp_tick;  // helper?
    cddp_cfg.start   = &sim_cddp_start;
    cddp_cfg.stop    = &sim_cddp_stop;

    // configure cddp helper functions
    cddp_cfg.initialized = &sim_cddp_initialized;
    cddp_cfg.connected   = &sim_cddp_connected;

    // flag simulated hardware interface as initialized
    s_init = true;

    printf("Sim CDDP initialized.\n");
}
