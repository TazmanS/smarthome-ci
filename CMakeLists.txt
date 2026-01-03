cmake_minimum_required(VERSION 3.16)

# -------------------------
# Project
# -------------------------
project(smarthome_ci C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# -------------------------
# Sources
# -------------------------
file(GLOB_RECURSE PROJECT_SOURCES
    ${CMAKE_SOURCE_DIR}/src/*.c
)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

# -------------------------
# Include directories
# -------------------------
target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src
)

# -------------------------
# Compiler warnings
# -------------------------
target_compile_options(${PROJECT_NAME}
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
)

# -------------------------
# Dependencies via pkg-config
# -------------------------
find_package(PkgConfig REQUIRED)

# -------------------------
# cJSON
# -------------------------
pkg_check_modules(CJSON REQUIRED libcjson)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${CJSON_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        ${CJSON_LIBRARIES}
)

# -------------------------
# PostgreSQL (libpq)
# -------------------------
pkg_check_modules(LIBPQ REQUIRED libpq)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${LIBPQ_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        ${LIBPQ_LIBRARIES}
)

# -------------------------
# Paho MQTT
# -------------------------
find_library(PAHO_MQTT_LIB paho-mqtt3c REQUIRED)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        ${PAHO_MQTT_LIB}
)

# -------------------------
# Threads (pthread)
# -------------------------
find_package(Threads REQUIRED)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        Threads::Threads
)

# -------------------------
# Debug flags
# -------------------------
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(${PROJECT_NAME}
        PRIVATE
            -O0
            -g
    )
endif()
