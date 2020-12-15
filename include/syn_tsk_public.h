#ifndef _SYN_TSK_PUBLIC_H_GUARD_
#define _SYN_TSK_PUBLIC_H_GUARD_

// includes
#include <stdint.h>
#include <stdlib.h>
#include <varargs.h>

#include "syn_mem_public.h"
#include "syn_sys_public.h"
#include "syn_typ_public.h"

// types

typedef void (*syn_tsk_tick_func)(syn_tsk_key_t key, ...);

// constants


syn_tsk_key_t syn_tsk_key_init(const char* tag, syn_tsk_tick_func func, size_t stack_size, uint8_t prot, uint8_t opt);

// synchronized task interface
int32_t syn_tsk_init();
int32_t syn_tsk_start(syn_mem_key_t key);
int32_t syn_tsk_join(syn_mem_key_t key);
int32_t syn_tsk_exit();

#endif /* _SYN_TSK_PUBLIC_H_GUARD_ */