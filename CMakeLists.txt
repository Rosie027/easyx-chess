cmake_minimum_required(VERSION 3.20)
project(easyx_chess LANGUAGES CXX)

if(NOT WIN32)
    message(FATAL_ERROR "easyx-chess requires Windows and the EasyX graphics library.")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(EASYX_INCLUDE_DIR "" CACHE PATH "Directory containing graphics.h")
set(EASYX_LIBRARY "" CACHE FILEPATH "Path to the EasyX library file")

if(NOT EXISTS "${EASYX_INCLUDE_DIR}/graphics.h")
    message(FATAL_ERROR "Set EASYX_INCLUDE_DIR to the directory containing graphics.h")
endif()

if(NOT EXISTS "${EASYX_LIBRARY}")
    message(FATAL_ERROR "Set EASYX_LIBRARY to the EasyX .lib file")
endif()

add_executable(easyx_chess
    src/main.cpp
    src/move.cpp
    src/checkmate.cpp
    src/ui.cpp
)

target_include_directories(easyx_chess PRIVATE include "${EASYX_INCLUDE_DIR}")
target_link_libraries(easyx_chess PRIVATE
    "${EASYX_LIBRARY}"
    User32
    Gdi32
    Shell32
)

if(MSVC)
    target_compile_options(easyx_chess PRIVATE /W4 /utf-8)
endif()
