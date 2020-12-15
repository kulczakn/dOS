#ifndef _SYN_MEM_PUBLIC_H_GUARD_
#define _SYN_MEM_PUBLIC_H_GUARD_

// includes
#include <stdint.h>
#include <stdlib.h>
#include <varargs.h>

#include "syn_sys_public.h"
#include "syn_typ_public.h"

// types

// constants

#define BIT0 SYNC_MEM_LOC_BIT

// synchronized memory interface
syn_mem_key_t syn_mem_key_init(const char* tag, size_t size, uint8_t prot, uint8_t opt);

int32_t syn_mem_get(syn_mem_key_t key);
int32_t syn_mem_release(syn_mem_key_t key);

#endif /* _SYN_MEM_PUBLIC_H_GUARD_ */