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
            pthread_attr_setstacksize( &s_lndp_thread_attr, SIM_LNDP_STACK_SIZE ) == 0 &&
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

    memset( lndp_driver, 0, sizeof( lndp_driver_t ) );

    /* Configure LNDP interface */
    lndp_driver->start = &sim_lndp_start;
    lndp_driver->stop  = &sim_lndp_stop;

    /* Configure LNDP helper functions */
    lndp_driver->initialized = &sim_lndp_initialized;
    lndp_driver->started     = &sim_lndp_started;

    /* Flag simulated hardware interface as initialized */
    s_init = true;

    success = true;

    printf("Sim LNDP initialized.\n");

    return success;
}
