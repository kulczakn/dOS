#ifndef _PACKETS_H_GUARD_
#define _PACKETS_H_GUARD_

#include "stdint.h"
#include <stdio.h> 
#include <string.h>
#include <unistd.h>

#include <arpa/inet.h> 
#include <sys/socket.h> 

#include <SDL2/SDL_thread.h>

#include "types.h"

#define PACKET_QUEUE_LENGTH 32

typedef uint8_t packet_id_t;

enum packet_id {
    PACKET_ID_FIRST = 0,

    PACKET_FRAME_STATE_ID = PACKET_ID_FIRST,
    PACKET_FRAME_STATE_REPLY_ID,

    PACKET_ID_LAST,

    PACKET_ID_COUNT = PACKET_ID_LAST - PACKET_ID_FIRST
};


/***** Packet Definitions *****/

typedef struct {
    packet_id_t packet_id;
    uint8_t buffer[31 * sizeof(uint8_t)];
} unknown_packet_t;
// assert sizeof() == 32

typedef struct {
    packet_id_t packet_id;
    union {
        frame_state_t data;
        uint8_t raw[sizeof(frame_state_t)];
    };
} frame_state_packet_t;
// assert sizeof() == 32


typedef struct {
    packet_id_t packet_id;
    union {
        frame_state_reply_t data;
        uint8_t raw[sizeof(frame_state_reply_t)];
    };
} frame_state_reply_packet_t;
// assert sizeof() == 32

/***** Macros *****/


/***** Function Declarations *****/

typedef void (*packet_callback_t)(void*);

// packet format callbacks

int8_t packet_init(const char* host, uint32_t port);

void packet_register_callback(packet_id_t id, packet_callback_t callback);

void packet_enqueue(void* packet);

void packet_dequeue(void* packet);

void packet_close();

void packet_sent();

int8_t packet_queue_length();

void packet_start();

//  New Interface

uint8_t connect_to_server(const char* host);

uint8_t start_session(uint8_t server);
uint8_t join_session(uint8_t server, uint8_t session);
uint8_t close_session(uint8_t session);

void register_callback(uint8_t session, uint8_t packet_id, packet_callback_t callback);
uint8_t message_session(uint8_t session, uint8_t message_t, uint8_t *packet, )

#endif /* _PACKETS_H_GUARD_ */