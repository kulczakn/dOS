#include "sim_net_private.h"

static uint8_t s_wifi_socket;
static bool s_wifi_inited;
static bool s_wifi_started;
static net_wifi_mode_t s_wifi_mode;

static bool sim_net_wifi_initialized( void )
{
    return s_wifi_inited;
}


static bool sim_net_wifi_started( bool wait )
{
    /* If wait flagged, wait until wifi starts */
    while( wait && !s_wifi_started );

    return s_wifi_started;
}


static bool sim_net_wifi_stopped( bool wait )
{
    /* If wait flagged, wait until wifi stops */
    while( wait && s_wifi_started );

    return !s_wifi_started;
}


net_wifi_mode_t wifi_mode( void )
{
    return s_wifi_mode;
}


static bool sim_net_ap_start( void )
{
    /* Local variables */
    bool success = false;

    // TODO: create messaging attribute as simulated ap

    /* If wifi is initialized, flag ap as started */
    if( s_wifi_inited )
    {
        s_wifi_started = true;
        s_wifi_mode = NET_WIFI_MODE_AP;

        success = true;
    }

    return success;
}


static bool sim_net_apsta_start( void )
{
    /* Local variables */
    bool success = false;

    /* If wifi is initialized, flag ap and sta as started */
    if( s_wifi_inited )
    {
        s_wifi_started = true;
        s_wifi_mode = NET_WIFI_MODE_APSTA;

        success = true;
    }

    return success;
}



static bool sim_net_sta_start( void )
{
    /* Local variables */
    bool success = false;

    /* If wifi is initialized, flag sta as started */
    if( s_wifi_inited )
    {
        s_wifi_started = true;
        s_wifi_mode = NET_WIFI_MODE_STA;

        success = true;
    }

    return success;
}


static bool sim_net_wifi_stop( void )
{
    /* Local variables */
    bool success = false;

    if( s_wifi_started )
    {
        s_wifi_started = false;
        s_wifi_mode =  NET_WIFI_MODE_NONE;

        success = true;
    }

    return success;
}


static bool sim_net_sock_create( uint64_t addr, uint8_t* sock )
{
    /* Local variables */
    bool    success = false;
    uint8_t new_sock  = 0;
    struct sockaddr_in socket_address = { 0 };

    /* Open UDP socket */
    if( ( new_sock = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP ) ) != 0 )
    {
        /* If the UDP server socket was created, bind to INADDR_ANY */
        socket_address.sin_family      = AF_INET;
        socket_address.sin_addr.s_addr = htonl( addr ); 
        socket_address.sin_port        = htons( SIM_LNDP_SERVER_PORT ); 

        if( bind( new_sock, (const struct sockaddr_in *)&socket_address, sizeof(struct sockaddr_in) ) != 0 ) 
        { 
            /* If the socket binds, flag success */
            *sock = new_sock;
            success = true;
        } 
    }

    return success;
}


static bool sim_net_sock_delete( uint8_t sock )
{
    /* Local variables */
    bool success = false;

    if( setsockopt( sock, SOL_SOCKET, SO_REUSEADDR, &true, sizeof( int ) ) == 0 )
    {
        success = true;
    }

    return success;
}


static bool sim_net_sock_send( uint8_t sock, void* data, size_t size, uint64_t addr, uint8_t port )
{
    /* Local variables */
    bool success = false;
    static struct sockaddr_in broadcast_addr;

    broadcast_addr.sin_family = AF_INET;                 
    broadcast_addr.sin_addr.s_addr = htonl( addr );
    broadcast_addr.sin_port = htons( port );

    /* Send data on socket */
    if( sendto(sock, data, size, 0, ( struct sockaddr_in* )&broadcast_addr, sizeof( struct sockaddr_in ) ) == size )
    {
        success = true;
    }

    return success;
}


static bool sim_net_sock_read( uint8_t sock, void* data, size_t size, size_t* read, uint64_t* addr, uint8_t* port )
{
    /* Local variables */
    bool success = false;
    size_t bytes_read;
    static struct sockaddr_in msg_addr;

    /* Read a datagram from the socket */
    if( ( bytes_read = recvfrom( sock, data, size, MSG_WAITALL, ( struct sockaddr_in* )&msg_addr, sizeof( struct sockaddr_in ) ) ) > 0 )
    {
        /* If a datagram was read, update stats */
        *read = bytes_read;
        *addr = msg_addr.sin_addr.s_addr;
        *port = msg_addr.sin_port;

        success = true;
    }

    return success;
}


bool sim_wifi_init( net_driver_t* wifi_driver )
{
    /* Local variables */
    bool success = false;

    printf("\nSim Net Wifi initializing...\n");

    /* Clear static variables */
    s_wifi_started = false;
    s_wifi_inited = false;
    s_wifi_socket = 0;
    s_wifi_mode = NET_WIFI_MODE_NONE;

    /* Configure the driver */
    memset( &wifi_driver, 0, sizeof( net_driver_t ) );

    // wifi_driver->

    /* Initialize wifi */
    s_wifi_inited = true;

    printf("\nSim Net Wifi initialized\n");

    return success;
}