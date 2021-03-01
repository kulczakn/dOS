#ifndef _ERR_PUBLIC_H_
#define _ERR_PUBLIC_H_

#endif /* _ERR_PUBLIC_H_ */

/**
 *          CONSTANTS
 */

#define ERR_STRING_MAX_LEN 32

char err_string[ ERR_COUNT ][ ERR_STRING_MAX_LEN ] = {
    "Fail",
    "No Error"
};

/**
 *          TYPES
 */
typedef enum {
    ERR_FAIL,
    ERR_NONE,

    ERR_COUNT
} err_t;