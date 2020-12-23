# project CDDP (Connected device data protocol)
project(CDDP)

add_library(${PROJECT_NAME} src/cddp.c)

target_include_directories( ${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)