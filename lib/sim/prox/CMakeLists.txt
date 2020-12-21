# project Sim Prox (Simulated Proximity sensor)
project(Prox)

add_library(${PROJECT_NAME} src/sim_prox.c)

target_include_directories( ${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)