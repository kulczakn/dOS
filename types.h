#ifndef _TYPES_GUARD_H_
#define _TYPES_GUARD_H_

#include "stdint.h"

typedef struct {
    float x_analog;
    float y_analog;
    int32_t x_mouse;
    int32_t y_mouse;
    
} frame_input_t;


typedef struct {
    uint32_t frame_id;
    int32_t code;
    frame_input_t frame_input;
    
    uint8_t buf[7];

} frame_state_t;
// assert sizeof() == 31


typedef struct {
    uint32_t frame_id;
    int32_t code;

    uint8_t buf[23];

} frame_state_reply_t;
// assert sizeof() == 31

#endif /* _TYPES_GUARD_H_ */