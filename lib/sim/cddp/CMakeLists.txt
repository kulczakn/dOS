# project Sim CDDP (Simulated Connected device data protocol)
project(sim_CDDP)

add_library(${PROJECT_NAME} src/sim_cddp.c)

target_include_directories( ${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)