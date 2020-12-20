#include "dos_pkt_public.h"

// public interface

void pkt_init( pkt_intf intf )
{
    // clear local variables and buffers

    // connect to port

    // start task
        // send connect packet and wait for response
        // send interface abd wait for response
}

void pkt_start( pkt_cfg cfg, void* pkt )
{
    // set local configuration

    // set packet data and flag to begin sending
}


void pkt_set( void* pkt, pkt_stat* stat )
{
    // update local packet data and stats
}


void pkt_get( void* pkt, pkt_stat* stat )
{
    // copy local packet data and stats into buffers
}


void pkt_stop( pkt_cfg* cfg, pkt_stat* stat )
{
    // send stop packet and cease
}


void pkt_deinit()
{
    // stop task

    // send disconnect packet

    // close port

    // clear local variables and buffers
}

// private interface

// static functions

static void pkt_task_main()
{
    while( 1 ) {
        // read in state from port

        // update local state 

        // write local state to port if time
    }
}