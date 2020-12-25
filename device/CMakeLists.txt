cmake_minimum_required (VERSION 3.5)

# project dOS
project(dOS)

# add project sources
add_executable(${PROJECT_NAME} 
    src/client_main.c 
    src/dos_cddp.c
    src/dos_prox.c
)

# add library sources
add_subdirectory(${PROJECT_SOURCE_DIR}/lib/cddp)
add_subdirectory(${PROJECT_SOURCE_DIR}/lib/prox)
add_subdirectory(${PROJECT_SOURCE_DIR}/lib/sim/cddp)
add_subdirectory(${PROJECT_SOURCE_DIR}/lib/sim/prox)

# set project cmake precompiler flags
add_compile_definitions(_CMAKE_SIM_BUILD_=1)

target_include_directories( ${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include

    PUBLIC ${PROJECT_SOURCE_DIR}/lib/cddp/include
    PUBLIC ${PROJECT_SOURCE_DIR}/lib/prox/include
    # PUBLIC ${PROJECT_SOURCE_DIR}/lib/env/include
    # PUBLIC ${PROJECT_SOURCE_DIR}/lib/snsr/include

    PUBLIC ${PROJECT_SOURCE_DIR}/lib/sim/cddp/include
    PUBLIC ${PROJECT_SOURCE_DIR}/lib/sim/prox/include
    # PUBLIC ${PROJECT_SOURCE_DIR}/lib/sim/env/include
    # PUBLIC ${PROJECT_SOURCE_DIR}/lib/sim/snsr/include
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        CDDP
        PROX
        sim_CDDP
        sim_PROX
)