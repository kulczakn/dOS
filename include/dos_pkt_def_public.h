#ifndef _DOS_PKT_DEF_PUBLIC_H_
#define _DOS_PKT_DEF_PUBLIC_H_

#include "stdint.h"

typedef enum {
    PKT_ID_FIRST = 0,

    PKT_SYS_CONN_ID = PKT_ID_FIRST,
    PKT_SYS_DISCONN_ID,
    PKT_SYS_INTF_ID,

    PKT_SRV_ACK_ID,
    PKT_SRV_INTFACK_ID,

    PKT_DOS_ID_TEMP,
    PKT_DOS_ID_HUMID,
    PKT_DOS_ID_PROXIM,

    PKT_ID_LAST,
    PKT_ID_COUNT = PKT_ID_LAST - PKT_ID_FIRST
} pkt_id;

#endif /* _DOS_PKT_DEF_PUBLIC_H_ */