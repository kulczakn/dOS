#ifndef _SYN_TYP_PUBLIC_H_GUARD_
#define _SYN_TYP_PUBLIC_H_GUARD_

// constants

#define BIT0 0b00000001
#define BIT1 0b00000010
#define BIT2 0b00000100
#define BIT3 0b00001000
#define BIT4 0b00010000
#define BIT5 0b00100000
#define BIT6 0b01000000
#define BIT7 0b10000000

/**
 *  Key layout
 *  
 *  SSSS.TT.R0.R1R1.IIII
 * 
 *  S  = Key Session
 *  T  = Key Type (mem, mutex, semaphore ...)
 *  R0 = Reserved 0
 *  R1 = Reserved 1
 *  I  = Key Index
 * 
 **/
#define SYN_SESSION_KEY_MASK 0xFF000000
#define SYN_TYPE_KEY_MASK    0x00F00000
#define SYN_RES0_KEY_MASK    0x000F0000
#define SYN_RES1_KEY_MASK    0x0000FF00
#define SYN_IDX_KEY_MASK     0x000000FF

#define SYN_LOCAL_KEY_MASK   0x00000000


// types

typedef uint32_t syn_mem_key_t;
typedef uint32_t syn_mtx_key_t;
typedef uint32_t syn_sem_key_t;
typedef uint32_t syn_ses_key_t;
typedef uint32_t syn_tsk_key_t;

#endif /* _SYN_TYP_PUBLIC_H_GUARD_ */