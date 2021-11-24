add_library(SamV71SerialCcsds STATIC)
target_sources(SamV71SerialCcsds
  PRIVATE   samv71_serial_ccsds.c
  PUBLIC    samv71_serial_ccsds.h)

 target_include_directories(SamV71SerialCcsds
  PUBLIC    ${CMAKE_SOURCE_DIR}/src/RuntimeMocks
            ${CMAKE_SOURCE_DIR}/TASTE-Runtime-Common/src/RuntimeMocks
            ${CMAKE_SOURCE_DIR}/TASTE-SAMV71-Runtime/SAMV71-BSP/src/
            ${CMAKE_SOURCE_DIR}/TASTE-SAMV71-Runtime/src/Hal)

target_link_libraries(SamV71SerialCcsds
  PRIVATE   common_build_options
            TASTE::Escaper
  PUBLIC    SAMV71::Runtime::Hal
            SAMV71::Uart)
            # SAMV71::FreeRTOS)

add_format_target(SamV71SerialCcsds)

add_library(TASTE::SamV71SerialCcsds ALIAS SamV71SerialCcsds)
