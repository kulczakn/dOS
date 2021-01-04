/**
 * 
 *  FILE:       sim_cddp.c
 *  MODULE:     sim_CDDP
 * 
 *  DESC:       Simulated cddp interface using UNIX sockets
 * 
 **/

#include "sim_cddp_private.h"

// static variables

static uint8_t          s_conn;                 // socket connection
static cddp_data_tick_t s_tick;                 // current tick
static cddp_data_tick_t s_tick_init;            // initial tick

static bool             s_init;                 // initialization status
static bool             s_connected;            // connected status
static bool             s_started;              // task started status


static pthread_attr_t   s_cddp_thread_attr;     // task thread attributes
static pthread_t        s_cddp_thread_id;       // task thread id


/**
 *  @brief  If the tick has been initialized, sim_cddp_tick will update the update and return the current tick.
 *          Otherwise, it will return 0.
 *  
 **/
static cddp_data_tick_t sim_cddp_tick( void )
{
    // local variables
    cddp_data_tick_t ret = 0;

    if( s_tick_init )
    {
        // if tick is initialized, update tick
        s_tick = (cddp_data_tick_t) time( NULL ) - s_tick_init;

        ret = s_tick;
    }

    // return current tick or 0
    return ret;
}


/**
 *  @brief  sim_cddp_initialized returns the status of the low level module initialization
 **/
static bool sim_cddp_initialized( void )
{
    return s_init;
}


/**
 *  @brief  sim_cddp_connected returns the low level module connected status
 **/
static bool sim_cddp_connected( void )
{
    return s_connected;
}


/**
 *  @brief sim_cddp_started returns the low level task thread started status
 **/
static bool sim_cddp_started( void )
{
    return s_started;
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
	if( ( s_conn = socket(AF_INET, SOCK_STREAM, 0 ) ) != 0 ) 
	{ 
        // if the socket was created succesfully

        serv_addr.sin_family = AF_INET; 
        serv_addr.sin_port = htons( SIM_CDDP_SERV_PORT ); 
        
        // Convert IPv4 and IPv6 addresses from text to binary form 
        if( inet_pton( AF_INET, SIM_CDDP_SERV_ADDR, &serv_addr.sin_addr ) != -1 ) 
        {
            // if the address was able to be converted to a binary form

            if( connect( s_conn, (struct sockaddr *)&serv_addr, sizeof(serv_addr) ) != -1 ) 
            {
                // if the socket was able to connect

                // set static variables
                s_tick_init = (cddp_data_tick_t)time( NULL );
                s_connected = true;

                printf("Sim CDDP connected.\n");

                rc = 1;
            }
        }
    }
    
    return rc;
}


/**
 * @brief   sim_cddp_read reads data packet from the socket interface
 * 
 * @param   data    data to be sent
 * @param   size    size of the data
 * @param   read    number of bytes read
 * @param   timeout timeout in usec
 * @return int 
 */
static int sim_cddp_read( void* data, size_t size, size_t* read, size_t timeout )
{
    // local variables
    int rc = -1;
    size_t bytes_read;
    struct timeval tv;

    if( timeout )
    {
        // if a timeout is given configure the socket

        tv.tv_sec = timeout / 1000;
        tv.tv_usec = timeout % 1000;

        setsockopt( s_conn, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof( struct timeval ) );
    }

    if( ( bytes_read = recv(s_conn, data, size, 0) ) > 0 )
    {
        // if some data was recieved, write back how many bytes
        *read = bytes_read;

        if( *read == CDDP_DATA_SIZE )
        {
            // if a full packet was recieved, return 1

            rc = 1;
        }
    }

    if( timeout )
    {
        // if a timeout was given, clear the configuration

        tv.tv_sec = 0;
        tv.tv_usec = 0;

        setsockopt( s_conn, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof( struct timeval ) );
    }


    return rc;
}


/**
 * @brief   sim_cddp_send sends a data packet across the socket interface
 * 
 * @param   data  data to be sent
 * @param   size  size of the data
 * @return int 
 */
static int sim_cddp_send( void* data, size_t size )
{
    // local variables
    int rc = -1;

    if( send(s_conn, data, size, 0) == size )
    {
        rc = 1;
    }

    return rc;
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
    int rc = -1;

    if( s_init      &&
        !s_started
      )
    {
        // if initialized but not started, start task

        if( pthread_attr_init( &s_cddp_thread_attr ) == 0 && 
            pthread_attr_setstacksize( &s_cddp_thread_attr, SIM_CDDP_STACK_SIZE ) == 0 &&
            pthread_create( &s_cddp_thread_id, &s_cddp_thread_attr, f, NULL )
          )
        {
            rc = 1;
            printf("Sim CDDP started.\n");
        }

    }

    return rc;
}


/**
 *  @brief  sim_cddp_stop stops the module task.
 **/
static int sim_cddp_stop( void )
{
    // local variables
    int rc = -1;
    
    if( s_started )
    {
        // if the thread has started, stop it

        if( pthread_join(s_cddp_thread_id, NULL) )
        {
            s_started = false;
            rc = 1;
        }
    }

    return rc;
}

/**
 *  @brief  sim_cddp_init initializes the data for the the simulated cddp module.
 *          This function will clear the modules static variables and will set the module interface functions in the cddp_cfg object.
 *          After this function has been executed, the module task can be started.
 **/
int sim_cddp_init( cddp_cfg_t* cddp_cfg )
{
    // local variables
    int rc = -1;

    printf("\nSim CDDP initializing...\n");

    // clear static variables
    s_conn = 0;
    s_tick = 0;
    s_tick_init = 0;

    memset( cddp_cfg, 0, sizeof( cddp_cfg_t ) );

    // configure cddp interface
    cddp_cfg->connect = &sim_cddp_connect;
    cddp_cfg->read    = &sim_cddp_read;
    cddp_cfg->send    = &sim_cddp_send;
    cddp_cfg->tick    = &sim_cddp_tick;  // helper?
    cddp_cfg->start   = &sim_cddp_start;
    cddp_cfg->stop    = &sim_cddp_stop;

    // configure cddp helper functions
    cddp_cfg->initialized = &sim_cddp_initialized;
    cddp_cfg->connected   = &sim_cddp_connected;
    cddp_cfg->started     = &sim_cddp_started;

    // flag simulated hardware interface as initialized
    s_init = true;

    rc = 1;

    printf("Sim CDDP initialized.\n");

    return rc;
}
