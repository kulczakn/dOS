cmake_minimum_required (VERSION 3.5)

# project dOS
project(dOS)

# add project sources
add_executable(${PROJECT_NAME} 
    src/client_main.c 
    src/dos_cddp.c
#    src/dos_lndp.c
    src/dos_snsr.c
)

# add library sources
add_subdirectory(${PROJECT_SOURCE_DIR}/lib/cddp)
add_subdirectory(${PROJECT_SOURCE_DIR}/lib/lndp)
add_subdirectory(${PROJECT_SOURCE_DIR}/lib/net)
add_subdirectory(${PROJECT_SOURCE_DIR}/lib/snsr)
add_subdirectory(${PROJECT_SOURCE_DIR}/lib/sim/cddp)
add_subdirectory(${PROJECT_SOURCE_DIR}/lib/sim/lndp)
add_subdirectory(${PROJECT_SOURCE_DIR}/lib/sim/net)
add_subdirectory(${PROJECT_SOURCE_DIR}/lib/sim/snsr)

# set project cmake precompiler flags
add_compile_definitions(_CMAKE_SIM_BUILD_=1)

target_include_directories( ${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include

    PUBLIC ${PROJECT_SOURCE_DIR}/lib/cddp/include
    PUBLIC ${PROJECT_SOURCE_DIR}/lib/lndp/include
    PUBLIC ${PROJECT_SOURCE_DIR}/lib/net/include
    PUBLIC ${PROJECT_SOURCE_DIR}/lib/snsr/include

    PUBLIC ${PROJECT_SOURCE_DIR}/lib/sim/cddp/include
    PUBLIC ${PROJECT_SOURCE_DIR}/lib/sim/lndp/include
    PUBLIC ${PROJECT_SOURCE_DIR}/lib/sim/net/include
    PUBLIC ${PROJECT_SOURCE_DIR}/lib/sim/snsr/include
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        CDDP
        SNSR
        LNDP
        NET
        sim_CDDP
        sim_SNSR
        sim_LNDP
        sim_NET
)