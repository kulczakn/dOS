#ifndef _SYN_SES_PUBLIC_H_GUARD_H_
#define _SYN_SES_PUBLIC_H_GUARD_H_

#include <stdint.h>
#include <stdlib.h>

#include "syn_typ_public.h"

// functions

syn_ses_key_t syn_ses_key_init(const char* tag, size_t size, uint8_t prot, uint8_t opt);

int32_t syn_ses_init();
int32_t syn_ses_join(syn_ses_key_t key);
int32_t syn_ses_exit();

// int32_t syn_ses_start();
// int32_t syn_ses_end();


#endif /* _SYN_SES_PUBLIC_H_GUARD_H_ */