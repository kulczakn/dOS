# project NET (Networking)
project(NET)

add_library(${PROJECT_NAME} src/net.c)

target_include_directories( ${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)