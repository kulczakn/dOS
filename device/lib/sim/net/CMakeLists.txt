# project Sim NET (Simulated Networking)
project(sim_NET)

add_library(${PROJECT_NAME} 
    src/sim_net.c
)

target_include_directories( ${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)

# link high level modules
#target_link_libraries(${PROJECT_NAME}
#    PUBLIC
#)