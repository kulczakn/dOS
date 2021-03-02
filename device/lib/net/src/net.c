#include "net_private.h"

net_driver_t s_device_driver;

/**
 *          PUBLIC INTERFACE IMPLEMENTATION
 */

/* Wifi interface */

bool net_wifi_init( void )
{
    /* Local variables */
    bool    success = false;

    return success;
}


bool net_wifi_ap_start( void )
{
    /* Local variables */
    bool    success = false;

    return success;
}



bool net_wifi_apsta_start( void )
{
    /* Local variables */
    bool    success = false;

    return success;
}



bool net_wifi_sta_start( void )
{
    /* Local variables */
    bool    success = false;

    return success;
}



bool net_wifi_stop( void )
{
    /* Local variables */
    bool    success = false;

    return success;
}


/* Socket interface */

bool net_socket_create( uint64_t addr, uint8_t port, uint8_t* sock )
{
    return s_device_driver.sock_create( addr, port, sock );
}


bool net_socket_delete( uint8_t sock )
{
    return s_device_driver.sock_delete( sock );
}


bool net_socket_send( uint8_t sock, void* data, size_t size, uint64_t addr, uint8_t port )
{
    return s_device_driver.sock_send( sock, data, size, addr, port );
}


bool net_socket_read( uint8_t sock, void* data, size_t size, size_t* read, uint64_t* addr, uint8_t* port )
{
    return s_device_driver.sock_read( sock, data, size, read, addr, port );
}
