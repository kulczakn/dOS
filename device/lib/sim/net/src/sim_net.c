#include "sim_net_private.h"

static bool s_inited;
static bool ap_started;
static bool apsta_started;
static bool sta_started;

bool ( *wifi_initialized ) ( void ); 
bool ( *ap_started       ) ( void );
bool ( *apsta_started    ) ( void );
bool ( *sta_started      ) ( void );

static bool sim_net_sock_create( uint64_t addr, uint8_t* sock )
{
    /* Local variables */
    bool    success = false;
    uint8_t socket  = 0;
    struct sockaddr socket_address = { 0 };

    /* Open UDP socket */
    if( ( new_socket = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP ) ) != 0 )
    {
        /* If the UDP server socket was created, bind to INADDR_ANY */
        socket_address.sin_family      = AF_INET;
        socket_address.sin_addr.s_addr = htonl( addr ); 
        socket_address.sin_port        = htons( SIM_LNDP_SERVER_PORT ); 

        if( bind( new_socket, (const struct sockaddr *)&socket_address, sizeof(struct sockaddr) ) != 0 ) 
        { 
            /* If the socket binds, flag success */
            *socket = new_socket;
            success = true;
        } 
    }

    return success;
}


static bool sim_net_sock_delete( uint8_t sock )
{
    /* Local variables */
    bool success = false;

    if( close( sock ) != 0 )
    {
        success = true;
    }

    return success;
}


static bool sim_net_sock_send( void* data, size_t size )
{
    /* Local variables */
    bool success = false;

    /* Send data on socket */
    if( sendto(s_socket, data, size, 0, ( struct sockaddr* )&s_broadcast_addr, sizeof( struct sockaddr ) ) == size )
    {
        success = true;
    }

    return success;
}


static bool sim_net_sock_read( void* data, size_t size, size_t* read )
{
    /* Local variables */
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
