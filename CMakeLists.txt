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
# cJSON (libcjson)
# -------------------------
find_package(PkgConfig REQUIRED)
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
# Paho MQTT 
# -------------------------
# include � library 
target_include_directories(${PROJECT_NAME} PRIVATE /usr/include)
target_link_libraries(${PROJECT_NAME} PRIVATE paho-mqtt3c)

# -------------------------
# Optional: debug flags
# -------------------------
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(${PROJECT_NAME}
        PRIVATE
            -O0
            -g
    )
endif()
