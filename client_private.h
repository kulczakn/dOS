#ifndef _CLIENT_PRIVATE_H_GUARD_
#define _CLIENT_PRIVATE_H_GUARD_

#include <stdint.h>
#include <stdlib.h>
#include <pthread.h>

#include "client_public.h"

#define CLIENT_Q_SIZE 64
#define CLIENT_CALLBACKS_SIZE 256
#define CLIENT_THREAD_STACK_SIZE 4096

typedef struct {
    uint32_t front;
    uint32_t back;
    client_generic_packet_t data[CLIENT_Q_SIZE];
} client_q_t;

// client queue
int32_t client_enq_packet(client_q_t * p_q, client_generic_packet_t * p_packet);
int32_t client_deq_packet(client_q_t * p_q, client_generic_packet_t * p_packet);
int32_t client_q_size(client_q_t * p_q);
int32_t client_q_clear(client_q_t * p_q);

// client network
int32_t client_send_packet(client_generic_packet_t * p_packet);

// client process
int32_t client_init(const char* host);
int32_t client_start();
int32_t client_stop();

#endif /* _CLIENT_PRIVATE_H_GUARD_ */