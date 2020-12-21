# project Prox (Proximity Sensor)
project(Prox)

add_library(${PROJECT_NAME} src/prox.c)

target_include_directories( ${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)