/**
 * 
 *      HIGH LEVEL CDDP INTERFACE
 * 
 */

#include "cddp_private.h"

// static

static void*         s_cddp_task( void* arg );  // cddp module processing task
static cddp_cfg_t    s_cddp_cfg;                // pointers to cddp IO interface 
static cddp_intrf_t* s_cddp_intrf;              // pointer to cddp data buffer in project side memory, MUST be size sizeof( cddp_intrf_t ) * CDDP_DATA_ID_COUNT
static uint64_t      s_intrf_count;             // current total enabled interfaces

static bool      s_handshook;                   // handshake completed flag

static uint8_t*  s_device;                      // pointer to device type enum saved in cddp buffer
static int32_t*  s_addr;                        // pointer to device address saved in cddp buffer

// public interface implementation

/**
 * @brief   cddp_init initializes the high level cddp module
 * 
 * @param   cfg         low level module function pointers
 * @param   buf         cddp data buffer in project side memory  
 * @param   buf_size    size of data buffer, must be sizeof( cddp_intrf_t ) * CDDP_DATA_ID_COUNT
 * 
 * @return int 
 */
int cddp_init( cddp_cfg_t* cddp_cfg, cddp_intrf_t* cddp_intrf, size_t buf_size )
{
    // local variables
    int rc = -1;
    cddp_conn_data_t* conn_data;
    cddp_connack_data_t* connack_data;

    // validate input
    if( cddp_cfg              &&
        cddp_cfg->connect     &&
        cddp_cfg->read        &&
        cddp_cfg->start       &&
        cddp_cfg->stop        &&
        cddp_cfg->send        &&
        cddp_cfg->connected   &&
        cddp_cfg->initialized &&
        cddp_cfg->started     &&
        cddp_cfg->tick        &&
        cddp_intrf            &&
        buf_size == sizeof( cddp_intrf_t ) 
                  * CDDP_DATA_ID_COUNT
     )
    {
        // if the low level interface has been configured and the data buffer is good, check if the low level module has been initialized

        if( cddp_cfg->initialized() )
        {
            // get pointers
            conn_data    = ( cddp_conn_data_t* )&cddp_intrf[ CDDP_SYS_DATA_CONN ].data;
            connack_data = ( cddp_connack_data_t* )&cddp_intrf[ CDDP_SYS_DATA_CONNACK ].data;

            // if low level module initialized, copy data into static variables
            s_cddp_cfg    = *cddp_cfg;
            s_cddp_intrf  = cddp_intrf;
            s_intrf_count = 0;

            s_handshook = false;

            s_addr   = &connack_data->addr;
            s_device = &conn_data->device;

            rc = 1;
        }
    }
    
    return rc;
}


/**
 * @brief   cddp_start starts the cddp module task using the low level 
 * 
 * @return int 
 */
int cddp_start( void )
{
    // local variables
    int rc = -1;

    if( s_cddp_cfg.initialized() )
    {
        //  if the module has been initialized, start it

        if( s_cddp_cfg.start( s_cddp_task ) > 0 )
        {
            // if start does not return an error, check if it has successfully started just in case

            if( s_cddp_cfg.started() > 0 )
            {
                rc = 1;
            }
        }
    }

    return rc;
}


/**
 * @brief   cddp_stop stops the cddp module task
 * 
 * @return int 
 */
int cddp_stop( void )
{
    // local variables
    int rc = -1;

    if( s_cddp_cfg.stop() )
    {
        // if attempting to stop does not create an error

        if( !s_cddp_cfg.started() )
        {
            // ensure task has actually stopped

            rc = 1;
        }
    }

    return rc;
}


/**
 * @brief   cddp_intrf_enable enables a new interface id
 * 
 * @param   id  data id to enable
 */
int cddp_intrf_enable( uint8_t id, uint8_t frmt, bool wrtbl, uint32_t count, uint32_t size, const char* name )
{
    // local variables
    int rc = -1;

    // validate input
    if( id >= CDDP_DATA_ID_FIRST && 
        id <  CDDP_DATA_ID_LAST  &&
        !s_cddp_intrf[ id ].enabled
      )
    {
        s_cddp_intrf[ id ].intrf.frmt  = frmt;
        s_cddp_intrf[ id ].intrf.wrtbl = wrtbl;
        s_cddp_intrf[ id ].intrf.count = count;
        s_cddp_intrf[ id ].intrf.size  = size;
        strncpy( s_cddp_intrf[ id ].intrf.name, name, 16 );

        s_cddp_intrf[ id ].enabled = true;
        s_intrf_count++;

        rc = 1;
    }

    return rc;
}


/**
 * @brief   cddp_data_disable disables a data id
 * 
 * @param   id  data id to disable
 */
int cddp_intrf_disable( uint8_t id )
{
    // local variables
    int rc = -1;

    // validate input
    if( id >= CDDP_DATA_ID_FIRST && 
        id <  CDDP_DATA_ID_LAST  &&
        s_cddp_intrf[ id ].enabled
      )
    {
        // disable data id
        memset( &s_cddp_intrf[ id ].intrf, 0, sizeof( cddp_intrf_data_t ) );
        memset( &s_cddp_intrf[ id ].data, 0, CDDP_DATA_SIZE );

        s_cddp_intrf[ id ].enabled = false;
        s_intrf_count--;

        rc = 1;
    }

    return rc;
}


/**
 * @brief   cddp_data_set updates a data id's data
 * 
 * @param   id      data id to update
 * @param   data    data to copy in
 */
int cddp_data_set( cddp_data_id_t id, void* data )
{
    // local variables
    int rc = -1;

    // validate input
    if( id >= CDDP_DATA_ID_FIRST   && 
        id <  CDDP_DATA_ID_LAST    &&
        s_cddp_intrf[ id ].enabled &&
        data                     
      )
    {
        // if id and data are valid and id is enabled, copy data over
        memcpy( &s_cddp_intrf[ id ].data, data, CDDP_DATA_SIZE );

        // update 
        s_cddp_intrf[ id ].tick = s_cddp_cfg.tick();

        rc = 1;
    }

    return rc;
}


/**
 * @brief 
 * 
 * @param id 
 * @param data 
 * @param tick 
 */
int cddp_data_get( cddp_data_id_t id, void* data, uint64_t* tick )
{
    // local variables
    int rc = -1;

    // validate input
    if( id >= CDDP_DATA_ID_FIRST   && 
        id <  CDDP_DATA_ID_LAST    &&
        s_cddp_intrf[ id ].enabled &&
        data
      )
    {
        // copy data over
        memcpy( data, &s_cddp_intrf[ id ].data, CDDP_DATA_SIZE );
        memcpy( tick, &s_cddp_intrf[ id ].tick, sizeof( uint64_t ) );
        
        rc = 1;
    }
    
    return rc;
}


/**
 * @brief   cddp_connected returns the connected status for the cddp module
 * 
 * @return true 
 * @return false 
 */
bool cddp_connected( void )
{
    return s_cddp_cfg.connected();
}


// private interface implementation

// static functions


static int s_cddp_handshake( void )
{
    // local variables
    int    rc = -1;
    size_t bytes = 0;

    cddp_conn_data_t    conn_data    = { 0 };
    cddp_connack_data_t connack_data = { 0 };

    cddp_pkt_t       conn_pkt        = { 0 };
    cddp_pkt_t       connack_pkt     = { 0 };
    cddp_pkt_t       intrf_pkt       = { 0 };

    // perform CDDP handshake
    if( s_cddp_cfg.connected() &&
        !s_handshook
      )
    {

        // copy data
        conn_data.device      = *s_device;
        conn_data.intrf_count = s_intrf_count;

        // create conn packet
        conn_pkt.addr = CDDP_SYS_ADDR;
        conn_pkt.id   = CDDP_SYS_DATA_CONN;
        memcpy( conn_pkt.data, &conn_data, CDDP_DATA_SIZE );
        conn_pkt.tick = s_cddp_cfg.tick();

        if( s_cddp_cfg.send( &conn_pkt, CDDP_PKT_SIZE ) )
        {
            // if conn packet is successfully send, read CONNACK packet

            if( s_cddp_cfg.read( &connack_pkt, CDDP_PKT_SIZE, &bytes, CDDP_CONNACK_TIMEOUT ) &&
                bytes == CDDP_PKT_SIZE
              )
            {
                // if CONNACK packet is successfully read, get CONNACK data
                memcpy( &connack_data, connack_pkt.data, CDDP_DATA_SIZE );
                
                // update local data
                *s_addr = connack_data.addr;

                // create intrf packet
                intrf_pkt.addr = CDDP_SYS_ADDR;
                intrf_pkt.id   = CDDP_SYS_DATA_INTRF;
                intrf_pkt.tick = s_cddp_cfg.tick();

                // send interface packets
                for( size_t i = CDDP_DATA_ID_FIRST; i < CDDP_DATA_ID_LAST; i++ )
                {
                    if( s_cddp_intrf[ i ].enabled )
                    {    
                        // if the id is enabled, copy interface data into packet
                        memcpy( &intrf_pkt.data, &s_cddp_intrf[ i ].intrf, sizeof( cddp_intrf_data_t ) );
                        
                        // send packet
                        if( s_cddp_cfg.send( &intrf_pkt, CDDP_PKT_SIZE ) )
                        {
                            continue;
                        }
                    }
                }

                // read final CONNACK packet
                if( s_cddp_cfg.read( &connack_pkt, CDDP_PKT_SIZE, &bytes, CDDP_CONNACK_TIMEOUT ) &&
                    bytes == CDDP_PKT_SIZE
                  )
                {
                    // if CONNACK packet is successfully read, get CONNACK data
                    memcpy( &connack_data, connack_pkt.data, CDDP_DATA_SIZE );

                    // check to make sure all interfaces were received
                    if( s_intrf_count == connack_data.intrf_cnt )
                    {
                        // flag handshake as completed
                        s_handshook = true;

                        rc = 1;
                    }
                }
            }
        }
    }

    return rc;
}


static int s_cddp_pkt_handle( cddp_pkt_t pkt )
{
    // local variables
    int rc = -1;
    cddp_intrf_t* buf;

    // handle packet

    if( pkt.id >= CDDP_DATA_ID_FIRST &&
        pkt.id <  CDDP_DATA_ID_LAST  &&
        pkt.addr | *s_addr 
      )
    {
        // special cases
        if( pkt.id == CDDP_SYS_DATA_KICK )
        {
            rc = CDDP_SYS_DATA_KICK;
        }
        else
        {
            if( s_cddp_intrf[ pkt.id ].enabled && 
                s_cddp_intrf[ pkt.id ].intrf.wrtbl
              )
            {
                // if data is enabled and writeable, copy over
                memcpy( &s_cddp_intrf[ pkt.id ].data, &pkt.data, CDDP_DATA_SIZE );
                s_cddp_intrf[ pkt.id ].tick = pkt.tick;

                rc = 1;
            }
        }
    }


    return rc;
}


static int s_cddp_handle_kick( void )
{
    // local variables
    int rc = -1;

    // close connection and set a flag

    return rc;
}


static void* s_cddp_task( void* arg )
{
    // local variables
    int         err    = CDDP_ERR_NONE;         // current error, 0 is none, -1 is fatal
    int         pkt_rc = 0;                     // return code from handled packet  
    size_t      bytes  = 0;                     // bytes read 
    cddp_pkt_t  cddp_pkt = { 0 };               // pointer to buffered packet data
    cddp_pkt_t  inc_pkt  = { 0 };               // incoming packet

    // connect to server
    if( s_cddp_cfg.connect() )
    {

        // if connect does not return error code
        if( s_cddp_handshake() &&
            s_handshook 
          )
        {
            // if handshake executes correctly

            // enter main loop
            while( err != -1 )
            {
                // read from socket
                if( s_cddp_cfg.read( &inc_pkt, CDDP_PKT_SIZE, &bytes, CDDP_TASK_TIMEOUT ) &&
                    bytes == CDDP_PKT_SIZE
                  )
                {
                    // if some data is read before the timeout
                    pkt_rc = s_cddp_pkt_handle( inc_pkt );

                    if( pkt_rc == CDDP_SYS_DATA_KICK )
                    {
                        err = CDDP_ERR_KICK;
                    }
                    
                }

                if( err == CDDP_ERR_NONE )
                {
                    // if incoming data has not caused any errors

                    // scan through each packet to see if it's activated
                    for( size_t i = CDDP_DATA_ID_FIRST; i < CDDP_DATA_ID_LAST; i++ )
                    {
                        // send active packets
                        if( s_cddp_intrf[ i ].enabled )
                        {
                            cddp_pkt.addr = CDDP_SYS_ADDR;
                            cddp_pkt.id   = i;
                            cddp_pkt.tick = s_cddp_cfg.tick();

                            memcpy( &cddp_pkt.data, &s_cddp_intrf[ i ].data, CDDP_DATA_SIZE );

                            if( s_cddp_cfg.send( &cddp_pkt, CDDP_PKT_SIZE ) )
                            {
                                // if send does not return error code
                                
                                // nothing to do
                            }
                            else
                            {
                                err = CDDP_ERR_SEND_FAIL; // TODO: no more magic numbers
                            }
                        }
                    }
                }

                // handle errors
                switch ( err )
                {
                case CDDP_ERR_KICK:

                    // close connection
                    s_cddp_handle_kick();

                    break;

                case CDDP_ERR_SEND_FAIL:

                    // clear error
                    err = CDDP_ERR_NONE;

                    break;
                
                default:
                    break;
                }
            }
        }
    }
  
    
}