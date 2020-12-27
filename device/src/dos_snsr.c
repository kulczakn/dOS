#include "dos_snsr_public.h"

// #include "sim_snsr_public.h"

#include "snsr_public.h"

#include "sim_snsr_dist_public.h"
#include "sim_snsr_public.h"

// project side SNSR variables

snsr_cfg_t      dos_snsr_cfgs[ DOS_SNSR_SNSR_COUNT ];
snsr_data_buf_t dos_snsr_buf[ DOS_SNSR_SNSR_COUNT ];

/**
 * @brief   dos_snsr_init 
 * 
 */
int dos_snsr_init( void )
{
    // local variables
    int rc = -1;

    snsr_cfg_t*         snsr_dist_cfg = &dos_snsr_cfgs[ 0 ];
    snsr_start_task_t   start_task;
    snsr_stop_task_t    stop_task;

    // init low level driver
    if(    
        #ifdef _CMAKE_SIM_BUILD_

        sim_snsr_init( &start_task, &stop_task ) && // init main sim driver
        sim_snsr_dist_init( snsr_dist_cfg )         // init distance sensor sim driver

        #else 
        // init esp32 hardware driver

        #endif
    )
    {
        // if all low level drivers initialize successfully

        if( snsr_init( dos_snsr_cfgs, start_task, stop_task, DOS_SNSR_SNSR_COUNT, dos_snsr_buf, DOS_SNSR_SNSR_COUNT * sizeof( snsr_data_buf_t ) ) )
        {
            // if the high level module initializes

            rc = 1;
        }
    }

    return rc;
}