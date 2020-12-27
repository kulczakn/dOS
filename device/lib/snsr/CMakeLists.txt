# project SNSR (Sensor)
project(SNSR)

add_library(${PROJECT_NAME} src/snsr.c)

target_include_directories( ${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)

# link high level modules
target_link_libraries(${PROJECT_NAME}
    PUBLIC
        CDDP
        pthread
)