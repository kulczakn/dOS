#ifndef _SYN_SEM_PUBLIC_H_GUARD_
#define _SYN_SEM_PUBLIC_H_GUARD_

#include <stdint.h>

// functions

syn_sem_key_t syn_sem_key_init(const char* tag, uint8_t prot, uint8_t opt);

int32_t syn_sem_get();
int32_t syn_sem_wait(syn_sem_key_t key);
int32_t syn_sem_post(syn_sem_key_t key);
int32_t syn_sem_release();


#endif /* _SYN_SEM_PUBLIC_H_GUARD_ */