cmake_minimum_required (VERSION 3.5)

# project dOS
project(dOS)

# add project sources
add_executable(${PROJECT_NAME} src/client_main.c )

# add library sources
add_subdirectory(${PROJECT_SOURCE_DIR}/lib/cddp)

target_include_directories( ${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
    PUBLIC ${PROJECT_SOURCE_DIR}/lib/cddp/include
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        CDDP
)