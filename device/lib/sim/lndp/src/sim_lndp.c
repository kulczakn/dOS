/**
 * 
 *  FILE:       sim_lndp.c
 *  MODULE:     sim_LNDP
 * 
 *  DESC:       LNDP driver interface for simulator using UNIX sockets
 * 
 **/

#include "sim_lndp_private.h"

// static variables

static uint8_t s_socket;
static struct sockaddr_in s_broadcast_addr;
static struct sockaddr_in s_server_addr;

static bool     s_init; 
static bool     s_started;

static pthread_attr_t s_lndp_thread_attr;
static pthread_t      s_lndp_thread_id;


/**
 *  @brief  sim_lndp_initialized returns the status of the low level module initialization
 **/
static bool sim_lndp_initialized( void )
{
    return s_init;
}


/**
 *  @brief sim_lndp_started returns the low level task thread started status
 **/
static bool sim_lndp_started( void )
{
    return s_started;
}


/**
 * @brief   sim_lndp_open_socket will open the UDP socket
 * 
 * @return  bool
 */
static bool sim_lndp_open_socket( void )
{
    /* local variables  */
    bool  success = false;

    /* Open UDP socket */

    if( ( s_socket = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP ) ) != 0 )
    {
        /* If the UDP server socket was created, bind to INADDR_ANY */

        if( bind( s_socket, (const struct sockaddr *)&s_server_addr, sizeof(struct sockaddr_in) ) != 0 ) 
        { 
            /* If the socket binds, flag success */

            success = true;
        } 
    }

    /* return result */
    return success;
}


/**
 * @brief   sim_lndp_read reads data packet from the socket interface
 * 
 * @param   data    data to be sent
 * @param   size    max size of the data
 * @param   read    number of bytes read
 * @param   timeout timeout in usec
 * @return  bool 
 */
static bool sim_lndp_read( void* data, size_t size, size_t* read )
{
    // local variables
    bool success = false;
    size_t bytes_read;

    /* Read a datagram from the socket */
    if( ( bytes_read = recvfrom( s_socket, data, size, MSG_WAITALL, (struct sockaddr*)&s_server_addr, sizeof(sockaddr) ) ) > 0 )
    {
        /* If a datagram was read, update stats */
        *read = bytes_read;
        success = true;
    }

    return success;
}


/**
 * @brief   sim_lndp_send sends a data packet across the socket interface
 * 
 * @param   data  data to be sent
 * @param   size  size of the data
 * @return  bool 
 */
static bool sim_lndp_send( void* data, size_t size )
{
    /* Local variables */
    int success = false;

    if( sendto(s_socket, data, size, 0, (struct sockaddr *)&s_broadcast_addr, sizeof(struct sockaddr_in) ) == size )
    {
        success = true;
    }

    return success;
}


/**
 *  @brief  sim_lndp_start starts the module task.
 *          After this function has been executed, the device code will be able to access the lndp data interface.
 *          
 *   @param f  Function that will be executed as the module task
 **/
static bool sim_lndp_start( void* ( *f ) ( void* ) )
{   
    printf("\nSim LNDP task starting...\n");

    // local variables
    bool success = true;

    if( s_init      &&
        !s_started
      )
    {
        // if initialized but not started, start task

        if( pthread_attr_init( &s_lndp_thread_attr ) == 0 && 
            pthread_attr_setstacksize( &s_lndp_thread_attr, sim_lndp_STACK_SIZE ) == 0 &&
            pthread_create( &s_lndp_thread_id, &s_lndp_thread_attr, f, NULL )
          )
        {
            success = true;
            printf("Sim LNDP started.\n");
        }

    }

    return success;
}


/**
 *  @brief  sim_lndp_stop stops the module task.
 **/
static bool sim_lndp_stop( void )
{
    // local variables
    bool success = false;
    
    if( s_started )
    {
        // if the thread has started, stop it

        if( pthread_join(s_lndp_thread_id, NULL) )
        {
            s_started = false;
            success = true;
        }
    }

    return success;
}

/**
 *  @brief  sim_lndp_init initializes the data for the the simulated lndp module.
 *          This function will clear the modules static variables and will set the module interface functions in the lndp_driver object.
 *          After this function has been executed, the module task can be started.
 **/
bool sim_lndp_init( lndp_driver_t* lndp_driver )
{
    /* Local variables */
    bool success = false;

    printf("\nSim LNDP initializing...\n");

    /* Clear static variables */
    s_socket = 0;

    /* Initialize address */
    s_server_addr.sin_family      = AF_INET;
    s_server_addr.sin_addr.s_addr = htonl( INADDR_ANY ); 
    s_server_addr.sin_port        = htons( SIM_LNDP_SERVER_PORT ); 

    s_broadcast_addr.sin_family = AF_INET;
    s_broadcast_addr.sin_addr.s_addr = htonl( INADDR_BROADCAST );
    s_broadcast_addr.sin_port   = htons( SIM_LNDP_SERVER_PORT );

    memset( lndp_driver, 0, sizeof( lndp_driver_t ) );

    /* Attempt to open socket */
    if( sim_lndp_open_socket() )
    {
        /* If socket was opened, configure LNDP interface */
        lndp_driver->read  = &sim_lndp_read;
        lndp_driver->send  = &sim_lndp_send;
        lndp_driver->start = &sim_lndp_start;
        lndp_driver->stop  = &sim_lndp_stop;

        /* Configure LNDP helper functions */
        lndp_driver->initialized = &sim_lndp_initialized;
        lndp_driver->started     = &sim_lndp_started;

        /* Flag simulated hardware interface as initialized */
        s_init = true;

        success = true;

        printf("Sim LNDP initialized.\n");
    }

    return success;
}
