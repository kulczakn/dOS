#ifndef _NET_PUBLIC_H_
#define _NET_PUBLIC_H_

#include "err_public.h"

/**
 *          TYPES
 */

typedef struct
{
    /* Wifi interface */
    bool ( *ap_start    ) ( void );
    bool ( *apsta_start ) ( void );
    bool ( *sta_start   ) ( void );
    bool ( *stop        ) ( void );

    bool ( *wifi_initialized ) ( void ); 
    bool ( *ap_started       ) ( void );
    bool ( *apsta_started    ) ( void );
    bool ( *sta_started      ) ( void );

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
bool net_socket_create( void );
bool net_socket_delete( void );
bool net_socket_send( void );
bool net_socket_read( void );

#endif /* _NET_PUBLIC_H_ */