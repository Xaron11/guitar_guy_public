cmake_minimum_required(VERSION 3.16)
project(c_game C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(ASSETS_DIR ${CMAKE_SOURCE_DIR}/assets)

# if(NOT CMAKE_BUILD_TYPE)
#   set(CMAKE_BUILD_TYPE Release)
# endif()

find_package(raylib 5 QUIET)

if(NOT raylib_FOUND)
    include(FetchContent)
    FetchContent_Declare(raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG master
        GIT_PROGRESS TRUE
    )
    FetchContent_MakeAvailable(raylib)
endif()

file(GLOB_RECURSE SOURCES src/*.c)
file(GLOB_RECURSE HEADERS src/*.h)

file(COPY ${ASSETS_DIR} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} raylib)

if (NOT WIN32)
    target_link_libraries(${PROJECT_NAME} m)
endif()

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR}/lib)
