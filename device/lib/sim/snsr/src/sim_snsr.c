#include "sim_snsr_private.h"

// static variables

static snsr_tick_t s_tick;                      // current tick
static snsr_tick_t s_tick_init;                 // initial tick

static bool             s_init;                 // initialization status
static bool             s_started;              // task started status


static pthread_attr_t   s_snsr_thread_attr;     // task thread attributes
static pthread_t        s_snsr_thread_id;       // task thread id

// static functions

int sim_snsr_start ( void* ( *f ) ( void* ) );
static int sim_snsr_stop( void );

// public interface

/**
 *  @brief
 * 
 *  @return int
 * 
 **/
int sim_snsr_init( snsr_start_task_t* snsr_start, snsr_stop_task_t* snsr_stop )
{
    // local variables
    int rc = -1;

    // clear static variables

    s_tick = 0;
    s_tick_init = 0;

    s_init    = false;
    s_started = false;

    memset(&s_snsr_thread_attr, 0, sizeof(pthread_attr_t) );
    s_snsr_thread_id   = 0;
    
    // configure start and stop tasks

    *snsr_start = &sim_snsr_start;
    *snsr_stop  = &sim_snsr_stop;

    // flag as initialized

    s_init = true;
    rc = 1;

    return rc;
}


/**
 *  @brief  If the tick has been initialized, sim_cddp_tick will update the update and return the current tick.
 *          Otherwise, it will return 0.
 *  
 **/
snsr_tick_t sim_snsr_tick( void )
{
    // local variables
    snsr_tick_t ret = 0;

    if( s_tick_init )
    {
        // if tick is initialized, update tick
        s_tick = (snsr_tick_t) time( NULL ) - s_tick_init;

        ret = s_tick;
    }

    // return current tick or 0
    return ret;
}


/**
 *  @brief  sim_cddp_initialized returns the status of the low level module initialization
 **/
bool sim_snsr_initialized( void )
{
    return s_init;
}


/**
 *  @brief sim_cddp_started returns the low level task thread started status
 **/
bool sim_snsr_started( void )
{
    return s_started;
}


/**
 *  @brief  sim_snsr_start starts the module task.
 *          After this function has been executed, the device code will be able to access the snsr data interface.
 *          
 *   @param f  Function that will be executed as the module task
 **/
int sim_snsr_start ( void* ( *f ) ( void* ) )
{   
    printf("\nSim SNSR starting...\n");

    // local variables
    int rc = -1;

    if( s_init     &&
        !s_started
      )
    {
        // if initialized, and not initialized, start task

        rc = pthread_attr_init( &s_snsr_thread_attr );
        rc = pthread_attr_setstacksize( &s_snsr_thread_attr, SIM_SNSR_STACK_SIZE );

        rc = pthread_create( &s_snsr_thread_id, &s_snsr_thread_attr, f, NULL );

        s_tick_init = (snsr_tick_t)time( NULL );

        rc = 1;

        printf("Sim SNSR started.\n");
    }

    return rc;
}


/**
 *  @brief  sim_snsr_stop stops the module task.
 **/
static int sim_snsr_stop( void )
{
    // local variables
    int rc = -1;
    
    if( s_started )
    {
        // if the thread has started, stop it

        if( pthread_join(s_snsr_thread_id, NULL) )
        {
            s_started = false;
            rc = 1;
        }
    }

    return rc;
}