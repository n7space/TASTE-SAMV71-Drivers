add_library(SamV71SerialCcsds STATIC)
target_sources(SamV71SerialCcsds
  PRIVATE   samv71_serial_ccsds.c
  PUBLIC    samv71_serial_ccsds.h)

 target_include_directories(SamV71SerialCcsds
  PUBLIC    ${CMAKE_SOURCE_DIR}/src/RuntimeMocks
            ${CMAKE_SOURCE_DIR}/TASTE-SAMV71-Runtime/src)

target_link_libraries(SamV71SerialCcsds
  PRIVATE   common_build_options
  PUBLIC    SAMV71::Runtime::Hal
            SAMV71::FreeRTOS
            TASTE::Broker
            TASTE::Escaper)

add_format_target(SamV71SerialCcsds)

add_library(TASTE::SamV71SerialCcsds ALIAS SamV71SerialCcsds)
