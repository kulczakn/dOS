# project Sim PROX (Simulated Proximity sensor)
project(sim_PROX)

add_library(${PROJECT_NAME} src/sim_prox.c)

target_include_directories( ${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)

# link high level modules
target_link_libraries(${PROJECT_NAME}
    CDDP
    PROX
)