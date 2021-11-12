add_library(SamV71SerialCcsds STATIC)
target_sources(SamV71SerialCcsds
 PRIVATE samv71_serial_ccsds.c
 PUBLIC  samv71_serial_ccsds.h)

target_link_libraries(SamV71SerialCcsds
  PRIVATE     common_build_options
              TASTE::RuntimeMocks)

add_format_target(SamV71SerialCcsds)

add_library(TASTE::SamV71SerialCcsds ALIAS SamV71SerialCcsds)
