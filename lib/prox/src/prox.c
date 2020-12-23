/**
 * 
 *      HIGH LEVEL Prox INTERFACE
 * 
 */

#include "prox_private.h"

// static


// public interface implementation

int prox_init( void )
{
    if( !prox_is_init )
    {
        return -1; // TODO
    }

    // clear flags
    
    return 1;
}


int prox_start( void )
{
    return prox_cfg.start();
}


int prox_stop( void )
{
    return prox_cfg.stop();
}


int prox_get( void* data, uint64_t tick )
{

}


// private interface implementation

// static functions