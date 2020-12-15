#include "syn_tsk_private.h"

// constants

// static variables
static pthread_attr_t    s_thread_attr;
static pthread_t         s_thread_id;

static syn_tsk_tick_func s_tsk_tick_funcs[SYN_TSK_MAX_TICK_FUNCS];
static uint8_t s_init;

static void sync_tsk_main();

// functions

syn_tsk_key_t syn_tsk_key_init(const char* tag, syn_tsk_tick_func func, size_t stack_size, uint8_t prot, uint8_t opt)
{
    // init guard
    if(!s_init)
    {
        // TODO
        return -1;
    }

    // input guard
    if(!tag || strlen(tag) == 0 || !func || stack_size == 0)
    {
        //TODO
        return -1;
    }

    // if tsk location is shared:
    //      if tag has been created in session:
    //          create local reference and return key
    //      else
    //          create a server instance
    //          create a local reference and return key 
    // else
    //      create local task instance
}


int32_t syn_tsk_init()
{
    // clear static variables
    memset(s_tsk_tick_funcs, 0, SYN_MAX_TICK_FUNCS * sizeof(syn_tsk_tick_func));

    // set init flag
    s_init = true;
}


int32_t syn_tsk_start(syn_mem_key_t key)
{
    // start task handler
}


int32_t syn_tsk_join(syn_mem_key_t key)
{
    // wait for the thread to exit
}


int32_t syn_tsk_exit()
{
    // close the local synchronized thread
}


void sync_tsk_init()
{
    // clear static variables
    memset(&s_thread_attr, 0, sizeof(pthread_attr_t));

    s_socket = 0;
}


void sync_tsk_start()
{
    int32_t rc = 0;

    rc = pthread_attr_init(&s_thread_attr);
    rc = pthread_attr_setstacksize(&s_thread_attr, SYNC_THREAD_STACK_SIZE);

    rc = pthread_create(&s_thread_id, &s_thread_attr, &sync_tsk_main, NULL);

    return 1;
}


void sync_tsk_end()
{
    int32_t rc = 0;
    
    rc = pthread_join(s_thread_id, NULL);

    return 1;
}


static void sync_tsk_main()
{
    // local variables

    // main task loop
    while(1)
    {

    }
}