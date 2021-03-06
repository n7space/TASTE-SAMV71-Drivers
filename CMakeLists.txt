cmake_minimum_required(VERSION 3.13 FATAL_ERROR)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/TASTE-SAMV71-Runtime/toolchains/samv71.cmake)
project(TASTE-SAMV71-Drivers VERSION 1.0.0 LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/build/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/build/lib)
set(TESTS_OUTPUT_PATH ${CMAKE_BINARY_DIR}/build/tests)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

include(MessageColors)
include(LoggingHelper)

add_library(common_build_options INTERFACE)
include(CommonBuildOptions)
include(Sanitizers)

option(TASTE_SAMV71_DRIVERS_WARNINGS_AS_ERRORS
       "Treat compiler warnings as errors"
       TRUE)

if(TASTE_SAMV71_DRIVERS_WARNINGS_AS_ERRORS)
    log_option_enabled("warnings as errors")
    set(CLANG_WARNINGS -Werror)
endif()

set(CLANG_WARNINGS ${CLANG_WARNINGS}
                   -Wall
                   -Wextra
                   -Wpedantic
                   -Wshadow
                   -Wcast-align
                   -Wunused
                   -Wsign-conversion
                   -Wnull-dereference
                   -Wdouble-promotion
                   -Wformat=2)

set(GCC_WARNINGS ${CLANG_WARNINGS}
                 -Wmisleading-indentation
                 -Wduplicated-cond
                 -Wduplicated-branches
                 -Wlogical-op)

log_info(${CMAKE_C_COMPILER_ID})

if(CMAKE_C_COMPILER_ID STREQUAL "Clang")
    set(PROJECT_WARNINGS ${CLANG_WARNINGS})
elseif(CMAKE_C_COMPILER_ID STREQUAL "GNU")
    set(PROJECT_WARNINGS ${GCC_WARNINGS})
endif()

target_compile_options(common_build_options INTERFACE ${PROJECT_WARNINGS})

add_library(bsp_build_options INTERFACE)
target_compile_definitions(bsp_build_options
    INTERFACE   USE_UART_IO
                LOW_LEVEL_IO_UART_ID=4)

include(CppCheck)
include(ClangTidy)
include(ClangFormat)
include(Doxygen)

add_subdirectory(TASTE-Runtime-Common/src/RuntimeMocks)
add_subdirectory(TASTE-Runtime-Common/src/Packetizer)
add_subdirectory(TASTE-Runtime-Common/src/Broker)
add_subdirectory(TASTE-Runtime-Common/src/Escaper)
add_subdirectory(TASTE-SAMV71-Runtime/SAMV71-BSP/src/Startup)
add_subdirectory(TASTE-SAMV71-Runtime/SAMV71-BSP/src/Nvic)
add_subdirectory(TASTE-SAMV71-Runtime/SAMV71-BSP/src/Wdt)
add_subdirectory(TASTE-SAMV71-Runtime/SAMV71-BSP/src/Fpu)
add_subdirectory(TASTE-SAMV71-Runtime/SAMV71-BSP/src/Sdramc)
add_subdirectory(TASTE-SAMV71-Runtime/SAMV71-BSP/src/Scb)
add_subdirectory(TASTE-SAMV71-Runtime/SAMV71-BSP/src/Pio)
add_subdirectory(TASTE-SAMV71-Runtime/SAMV71-BSP/src/Pmc)
add_subdirectory(TASTE-SAMV71-Runtime/SAMV71-BSP/src/Uart)
add_subdirectory(TASTE-SAMV71-Runtime/SAMV71-BSP/src/Utils)
add_subdirectory(TASTE-SAMV71-Runtime/src/Hal)
add_subdirectory(TASTE-SAMV71-Runtime/src/Init)
add_subdirectory(TASTE-SAMV71-Runtime/src/BrokerLock)
add_subdirectory(TASTE-SAMV71-Runtime/src/Xdmac)
add_subdirectory(TASTE-SAMV71-Runtime/FreeRTOS)
add_subdirectory(src)
