add_executable(Samv71Demo)
target_sources(Samv71Demo
    PRIVATE     main.c
                PacketSender.c
    PUBLIC      PacketSender.h)

target_include_directories(Samv71Demo
    PRIVATE     ${CMAKE_SOURCE_DIR}/src
                ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(Samv71Demo
    PRIVATE     common_build_options
                SAMV71::Runtime::Hal
                SAMV71::Runtime::Init
                SAMV71::FreeRTOS
                TASTE::SamV71SerialCcsds
                TASTE::Broker
                SAMV71::Runtime::BrokerLock)

target_link_options(Samv71Demo
    PRIVATE     -T${CMAKE_SOURCE_DIR}/TASTE-SAMV71-Runtime/SAMV71-BSP/ld/samv71q21_sram.ld)

add_format_target(Samv71Demo)
