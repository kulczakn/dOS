#ifndef _NET_PUBLIC_H_
#define _NET_PUBLIC_H_

// #include "err_public.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

#include <arpa/inet.h> 
#include <sys/socket.h> 
#include <sys/time.h>

/**
 *          TYPES
 */

typedef uint8_t net_wifi_mode_t;
enum
{
    NET_WIFI_MODE_NONE,
    NET_WIFI_MODE_AP,
    NET_WIFI_MODE_APSTA,
    NET_WIFI_MODE_STA,

    NET_WIFI_MODE_COUNT
};


typedef struct
{
    /* Wifi interface */
    bool ( *ap_start    ) ( void );
    bool ( *apsta_start ) ( void );
    bool ( *sta_start   ) ( void );
    bool ( *wifi_stop   ) ( void );

    bool ( *wifi_initialized ) ( void ); 
    bool ( *wifi_started     ) ( bool );

    net_wifi_mode_t ( *wifi_mode ) ( void );

    /* Socket interface */
    bool ( *sock_create ) ( void );
    bool ( *sock_delete ) ( void );
    bool ( *sock_read   ) ( void*, size_t, size_t*, size_t );
    bool ( *sock_send   ) ( void*, size_t );

} net_driver_t;

/**
 *          PUBLIC INTERFACE
 */

/* Wifi interface */
bool net_wifi_init( void );
bool net_wifi_ap_start( void );
bool net_wifi_apsta_start( void );
bool net_wifi_sta_start( void );
bool net_wifi_stop( void );

/* Socket interface */
bool net_socket_create( uint64_t addr, uint8_t* sock );
bool net_socket_delete( uint8_t sock );
bool net_socket_send( void* data, size_t size );
bool net_socket_read( void* data, size_t size, size_t* read );

#endif /* _NET_PUBLIC_H_ */