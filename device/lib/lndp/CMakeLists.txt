# project LNDP (Locally Networked Device Protocol)
project(LNDP)

add_library(${PROJECT_NAME} src/lndp.c)

target_include_directories( ${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)

# link high level modules
target_link_libraries(${PROJECT_NAME}
    PUBLIC
        NET
)