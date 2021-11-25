add_executable(Samv71Demo)
target_sources(Samv71Demo
    PRIVATE     main.c)
target_include_directories(Samv71Demo
    PRIVATE     ${CMAKE_SOURCE_DIR}/TASTE-SAMV71-Runtime/src
                ${CMAKE_SOURCE_DIR}/src/RuntimeMocks
                ${CMAKE_SOURCE_DIR}/src
                ${CMAKE_SOURCE_DIR}/TASTE-Runtime-Common/src/Escaper
                ${CMAKE_SOURCE_DIR}/TASTE-Runtime-Common/src/Broker
                ${CMAKE_SOURCE_DIR}/TASTE-Runtime-Common/src/Packetizer
                ${CMAKE_SOURCE_DIR}/TASTE-Runtime-Common/src/RuntimeMocks)

target_link_libraries(Samv71Demo
    PRIVATE     SAMV71::Runtime::BrokerLock
                SAMV71::Runtime::Hal
                SAMV71::Runtime::Init
                SAMV71::FreeRTOS
                TASTE::SamV71SerialCcsds
          )

target_link_options(Samv71Demo
    PRIVATE     -T${CMAKE_SOURCE_DIR}/TASTE-SAMV71-Runtime/SAMV71-BSP/ld/samv71q21_sram.ld)

add_format_target(Samv71Demo)
