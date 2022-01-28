/**@file
 * This file is part of the TASTE Linux Runtime.
 *
 * @copyright 2021 N7 Space Sp. z o.o.
 *
 * TASTE Linux Runtime was developed under a programme of,
 * and funded by, the European Space Agency (the "ESA").
 *
 * Licensed under the ESA Public License (ESA-PL) Permissive,
 * Version 2.3 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://essr.esa.int/license/list
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef SAMV71_SERIAL_CCSDS_H
#define SAMV71_SERIAL_CCSDS_H

/**
 * @file     samv71_serial_ccsds.h
 * @brief    Driver for TASTE with uses UART for communication
 */

#include "samv71_serial_ccsds_internal.h"
#include <Broker.h>
#include <Escaper.h>

#include <drivers_config.h>
#include <system_spec.h>

#include <stddef.h>

#include <Hal/Hal.h>

#include <FreeRTOS.h>
#include <semphr.h>
#include <task.h>

#define Serial_CCSDS_SAMV71_FIFO_BUFFER_SIZE 256
#define Serial_CCSDS_SAMV71_RECV_BUFFER_SIZE 256
#define Serial_CCSDS_SAMV71_ENCODED_PACKET_MAX_SIZE 256
#define Serial_CCSDS_SAMV71_DECODED_PACKET_MAX_SIZE BROKER_BUFFER_SIZE

/**
 * @brief Structure for driver internal data
 *
 * This structure is allocated by runtime and the pointer is passed to all
 * driver functions. The name of this structure shall match driver definition
 * from ocarina_components.aadl and has suffix '_private_data'.
 */
typedef struct final {
  Serial_CCSDS_SamV71_Device_T m_device;
  Hal_Uart m_hal_uart;
  Hal_Uart_Config m_hal_uart_config;
  uint8_t m_fifo_memory_block[Serial_CCSDS_SAMV71_FIFO_BUFFER_SIZE];
  uint8_t m_recv_buffer[Serial_CCSDS_SAMV71_RECV_BUFFER_SIZE];
  uint8_t m_encoded_packet_buffer[Serial_CCSDS_SAMV71_ENCODED_PACKET_MAX_SIZE];
  uint8_t m_decoded_packet_buffer[Serial_CCSDS_SAMV71_DECODED_PACKET_MAX_SIZE];
  Escaper m_escaper;
  TaskHandle_t m_task;
  StaticTask_t m_task_buffer;
  StackType_t m_task_stack_buffer[DRIVER_TASK_STACK_SIZE];
  Uart_RxHandler m_uart_rx_handler;
  SemaphoreHandle_t m_rx_semaphore;
  StaticSemaphore_t m_rx_semaphore_buffer;
  Uart_TxHandler m_uart_tx_handler;
  SemaphoreHandle_t m_tx_semaphore;
  StaticSemaphore_t m_tx_semaphore_buffer;
  Uart_ErrorHandler m_uart_error_handler;
} samv71_serial_ccsds_private_data;

/**
 * @brief Initialize driver.
 *
 * Function is used by runtime to initialize the driver.
 *
 * @param private_data                  Driver private data, allocated by
 * runtime
 * @param bus_id                        Identifier of the bus, which is driver
 * @param device_id                     Identifier of the device
 * @param device_configuration          Configuration of device
 * @param remote_device_configuration   Configuration of remote device
 */
void SamV71SerialCcsdsInit(
    void *private_data, const enum SystemBus bus_id,
    const enum SystemDevice device_id,
    const Serial_CCSDS_SamV71_Conf_T *const device_configuration,
    const Serial_CCSDS_SamV71_Conf_T *const remote_device_configuration);

/**
 * @brief Function which implements receiving data from remote partition.
 *
 * Functions works in separate thread, which is initialized by
 * SamV71SerialCcsdsSend
 *
 * @param private_data   Driver private data, allocated by runtime
 */
void SamV71SerialCcsdsPoll(void *private_data);

/**
 * @brief Send data to remote partition.
 *
 * Function is used by runtime.
 *
 * @param private_data   Driver private data, allocated by runtime
 * @param data           The Buffer which data to send to connected remote
 * partition
 * @param length         The size of the buffer
 */
void SamV71SerialCcsdsSend(void *private_data, const uint8_t *const data,
                           const size_t length);

#endif
