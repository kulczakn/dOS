# project Sim SNSR (Simulated Sensor)
project(sim_SNSR)

add_library(${PROJECT_NAME} 
    src/sim_snsr.c 
    src/sim_snsr_dist.c
)

target_include_directories( ${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)

# link high level modules
target_link_libraries(${PROJECT_NAME}
    PUBLIC
        CDDP
        SNSR
        pthread
)