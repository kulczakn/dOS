#ifndef _DOS_PKT_PUBLIC_H_
#define _DOS_PKT_PUBLIC_H_

#include "stdint.h"

#include "dos_pkt_def_public.h"

// types

typedef struct {
    uint8_t pkts_actv[ PKT_ID_COUNT ];
    uint8_t pkts_intrvl[ PKT_ID_COUNT ];
    uint8_t pkts_wrtble[ PKT_ID_COUNT ];
    char    pkts_tag[ PKT_ID_COUNT ][ 32 ];
} pkt_intf;


typedef struct {
    uint32_t id;
    uint8_t  tick_intrvl;
    char     tag[32];
} pkt_cfg;


typedef struct {
    uint32_t code; // 0 stopped 1 started
} pkt_stat;


// interface

void pkt_init( pkt_intf intf );

void pkt_start( pkt_cfg cfg, void* pkt );
void pkt_updt( void* pkt, pkt_stat* stat );
void pkt_get( void* pkt, pkt_stat* stat );
void pkt_stop( pkt_cfg* cfg, pkt_stat* stat );

void pkt_deinit();

#endif /* _DOS_PKT_PUBLIC_H_ */