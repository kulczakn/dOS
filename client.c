#include "client_private.h"



// queues
client_q_t s_client_in_q;
client_q_t s_client_out_q;

// network
static uint8_t s_socket;

// process
static client_callback_t s_client_callbacks[CLIENT_CALLBACKS_SIZE];
static pthread_attr_t    s_client_thread_attr;
static pthread_t         s_client_thread_id;

// static functions
static void * client_tick();



// client queue
int32_t client_enq_packet(client_q_t * p_q, client_generic_packet_t * p_packet)
{
    // check input
    if(!p_q || !p_packet)
    {
        return -1;
    }

    // TODO: CONSIDER
    //  - with this check here we can't overwrite our circular queue buffer
    //  - if we remove it then when you overflow it would simply start to overwrite buffer
    //  - if we dont      then if you start to overflow it would catch you and you couldn't write until the rest of the buffer had been cleared, protecting the data

    // if full
    if(client_q_size(p_q) < 0)
    {
        return 0;
    }

    // add to queue
    client_generic_packet_t * q_back = &p_q->data[p_q->back];
    memcpy(q_back, p_packet, sizeof(client_generic_packet_t));
    
    p_q->back = (p_q->back + 1) % CLIENT_Q_SIZE;

    return 1;
}


int32_t client_deq_packet(client_q_t * p_q, client_generic_packet_t * p_packet)
{
    // check input
    if(!p_q || !p_packet)
    {
        return -1;
    }
    
    // if the queue is empty or in a bad state
    if(client_q_size(p_q) <= 0)
    {
        return 0;
    }

    // remove from queue
    client_generic_packet_t * q_front = &p_q->data[p_q->front];
    memcpy(p_packet, q_front, sizeof(client_generic_packet_t));
    
    p_q->front = (p_q->front + 1) % CLIENT_Q_SIZE;

    return 1;
}


int32_t client_q_size(client_q_t * p_q)
{
    return p_q->back - p_q->front;
}


int32_t client_q_clear(client_q_t * p_q)
{
    memset(p_q, 0, sizeof(client_q_t));
    return 1;
}

// client network
int32_t client_send_packet(client_generic_packet_t * p_packet)
{
    send(s_socket, p_packet, sizeof(client_generic_packet_t), 0);

    return 1;
}


// client process
int32_t client_init(const char* host)
{
    // clear static variables
    memset(&s_client_in_q, 0, sizeof(client_q_t));
    memset(&s_client_out_q, 0, sizeof(client_q_t));
    memset(&s_client_callbacks, 0, CLIENT_CALLBACKS_SIZE * sizeof(client_callback_t));
    memset(&s_client_thread_attr, 0, sizeof(pthread_attr_t));

    s_socket           = 0;
    s_client_thread_id = 0;
}


int32_t client_start()
{
    int rc = 0;

    rc = pthread_attr_init(&s_client_thread_attr);
    rc = pthread_attr_setstacksize(&s_client_thread_attr, CLIENT_THREAD_STACK_SIZE);

    rc = pthread_create(&s_client_thread_id, &s_client_thread_attr, &client_tick, NULL);

    return 1;
}


int32_t client_stop()
{
    int rc = 0;
    
    rc = pthread_join(s_client_thread_id, NULL);

    return 1;
}

// main process
static void* client_tick()
{
    // local variables

    while(1)
    {
        // process incoming

        // send outgoing
    }
}