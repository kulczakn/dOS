#ifndef _CLIENT_PUBLIC_H_GUARD_
#define _CLIENT_PUBLIC_H_GUARD_

#include <stdint.h>
#include <stdlib.h>

// definitions
typedef void (*client_callback_t)(void);
typedef uint8_t packet_id_t;

// packets
typedef struct {
    packet_id_t packet_id;
    uint8_t buffer[31 * sizeof(uint8_t)];
} client_generic_packet_t;
// assert sizeof() == 32

#endif /* _CLIENT_PUBLIC_H_GUARD_ */`