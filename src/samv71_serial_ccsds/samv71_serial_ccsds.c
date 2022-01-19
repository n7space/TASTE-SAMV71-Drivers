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

#include "samv71_serial_ccsds.h"

#include <assert.h>

#include <EscaperInternal.h>

#define SAMV71_SERIAL_CCSDS_POOL_ERROR "Polling error! Fifo count <= 0."

Uart *uart0handle;
Uart *uart1handle;
Uart *uart2handle;
Uart *uart3handle;
Uart *uart4handle;

void UART0_Handler(void) {
  if (uart0handle != NULL)
    Uart_handleInterrupt(uart0handle);
}

void UART1_Handler(void) {
  if (uart1handle != NULL)
    Uart_handleInterrupt(uart1handle);
}

void UART2_Handler(void) {
  if (uart2handle != NULL)
    Uart_handleInterrupt(uart2handle);
}

void UART3_Handler(void) {
  if (uart3handle != NULL)
    Uart_handleInterrupt(uart3handle);
}

void UART4_Handler(void) {
  if (uart4handle != NULL)
    Uart_handleInterrupt(uart4handle);
}

static inline const char *
SamV71_device_to_string(const Serial_CCSDS_SamV71_Device_T device) {
  switch (device) {
  case uart0: {
    static const char uart0Name[] = "uart0";
    return uart0Name;
  } break;
  case uart1: {
    static const char uart1Name[] = "uart1";
    return uart1Name;
  } break;
  case uart2: {
    static const char uart2Name[] = "uart2";
    return uart2Name;
  } break;
  case uart3: {
    static const char uart3Name[] = "uart3";
    return uart3Name;
  } break;
  case uart4: {
    static const char uart4Name[] = "uart4";
    return uart4Name;
  } break;
  default:
    assert(false && "Not supported device name");
    return NULL;
  }
}

static inline void
SamV71SerialCcsdsInit_uart_register(samv71_serial_ccsds_private_data *self,
                                    Serial_CCSDS_SamV71_Device_T deviceName) {
  switch (deviceName) {
  case uart0:
    self->m_hal_uart_config.id = Uart_Id_0;
    break;
  case uart1:
    self->m_hal_uart_config.id = Uart_Id_1;
    break;
  case uart2:
    self->m_hal_uart_config.id = Uart_Id_2;
    break;
  case uart3:
    self->m_hal_uart_config.id = Uart_Id_3;
    break;
  case uart4:
    self->m_hal_uart_config.id = Uart_Id_4;
    break;
  default:
    assert(false && "Not supported device name");
  }
}

static inline void
SamV71SerialCcsdsInit_uart_handle(samv71_serial_ccsds_private_data *self,
                                  Serial_CCSDS_SamV71_Device_T deviceName) {
  switch (deviceName) {
  case uart0:
    uart0handle = &self->m_hal_uart.uart;
    break;
  case uart1:
    uart1handle = &self->m_hal_uart.uart;
    break;
  case uart2:
    uart2handle = &self->m_hal_uart.uart;
    break;
  case uart3:
    uart3handle = &self->m_hal_uart.uart;
    break;
  case uart4:
    uart4handle = &self->m_hal_uart.uart;
    break;
  default:
    assert(false && "Not supported device name");
  }
}

static inline void
SamV71SerialCcsdsInit_uart_data_bits(samv71_serial_ccsds_private_data *self,
                                     Serial_CCSDS_SamV71_Conf_T_bits bits) {
  (void)self;
  (void)bits;
  assert((bits == 8) && "Not supported number of data bits");
}

static inline void SamV71SerialCcsdsInit_uart_parity(
    samv71_serial_ccsds_private_data *self,
    Serial_CCSDS_SamV71_Conf_T_use_paritybit useParity,
    Serial_CCSDS_SamV71_Parity_T parity) {
  if (useParity) {
    switch (parity) {
    case Serial_CCSDS_SamV71_Parity_T_odd:
      self->m_hal_uart_config.parity = Uart_Parity_Odd;
      break;
    case Serial_CCSDS_SamV71_Parity_T_even:
      self->m_hal_uart_config.parity = Uart_Parity_Even;
      break;
    default:
      assert(false && "Not supported parity");
    }
  } else {
    self->m_hal_uart_config.parity = Uart_Parity_None;
  }
}

static inline void
SamV71SerialCcsdsInit_uart_baudrate(samv71_serial_ccsds_private_data *self,
                                    Serial_CCSDS_SamV71_Baudrate_T speed) {

  switch (speed) {
  case Serial_CCSDS_SamV71_Baudrate_T_b9600:
    self->m_hal_uart_config.baudrate = 9600;
    break;
  case Serial_CCSDS_SamV71_Baudrate_T_b19200:
    self->m_hal_uart_config.baudrate = 19200;
    break;
  case Serial_CCSDS_SamV71_Baudrate_T_b38400:
    self->m_hal_uart_config.baudrate = 38400;
    break;
  case Serial_CCSDS_SamV71_Baudrate_T_b57600:
    self->m_hal_uart_config.baudrate = 57600;
    break;
  case Serial_CCSDS_SamV71_Baudrate_T_b115200:
    self->m_hal_uart_config.baudrate = 115200;
    break;
  case Serial_CCSDS_SamV71_Baudrate_T_b230400:
    self->m_hal_uart_config.baudrate = 230400;
    break;
  default:
    assert(false && "Not supported baudrate");
    break;
  }
}

static inline void SamV71SerialCcsdsInit_uart_init(
    samv71_serial_ccsds_private_data *const self,
    const Serial_CCSDS_SamV71_Conf_T *const device_configuration) {
  SamV71SerialCcsdsInit_uart_register(self, device_configuration->devname);
  SamV71SerialCcsdsInit_uart_data_bits(self, device_configuration->bits);
  SamV71SerialCcsdsInit_uart_parity(self, device_configuration->use_paritybit,
                                    device_configuration->parity);
  SamV71SerialCcsdsInit_uart_baudrate(self, device_configuration->speed);
  SamV71SerialCcsdsInit_uart_handle(self, device_configuration->devname);
  Hal_uart_init(&self->m_hal_uart, self->m_hal_uart_config);
}

static void UartRxCallback(void *private_data) {
  samv71_serial_ccsds_private_data *self =
      (samv71_serial_ccsds_private_data *)private_data;

  xSemaphoreGiveFromISR(self->m_rx_semaphore, NULL);
}

static ByteFifo *UartTxCallback(void *private_data) {
  samv71_serial_ccsds_private_data *self =
      (samv71_serial_ccsds_private_data *)private_data;

  xSemaphoreGiveFromISR(self->m_tx_semaphore, NULL);
  return NULL;
}

static inline void
SamV71SerialCcsdsInit_rx_handler(samv71_serial_ccsds_private_data *const self) {
  self->m_uart_rx_handler.characterCallback = UartRxCallback;
  self->m_uart_rx_handler.lengthCallback = UartRxCallback;
  self->m_uart_rx_handler.lengthArg = self;
  self->m_uart_rx_handler.characterArg = self;
  self->m_uart_rx_handler.targetCharacter = STOP_BYTE;
  self->m_uart_rx_handler.targetLength = Serial_CCSDS_SAMV71_RECV_BUFFER_SIZE;
  self->m_rx_semaphore =
      xSemaphoreCreateBinaryStatic(&self->m_rx_semaphore_buffer);
  xSemaphoreGive(self->m_rx_semaphore);
}

static inline void
SamV71SerialCcsdsInit_tx_handler(samv71_serial_ccsds_private_data *const self) {
  self->m_uart_tx_handler.callback = UartTxCallback;
  self->m_uart_tx_handler.arg = self;
  self->m_tx_semaphore =
      xSemaphoreCreateBinaryStatic(&self->m_tx_semaphore_buffer);
  xSemaphoreGive(self->m_tx_semaphore);
}

void SamV71SerialCcsdsInit(
    void *private_data, const enum SystemBus bus_id,
    const enum SystemDevice device_id,
    const Serial_CCSDS_SamV71_Conf_T *const device_configuration,
    const Serial_CCSDS_SamV71_Conf_T *const remote_device_configuration) {
  (void)bus_id;
  (void)device_id;
  (void)remote_device_configuration;

  samv71_serial_ccsds_private_data *self =
      (samv71_serial_ccsds_private_data *)private_data;

  SamV71SerialCcsdsInit_uart_init(self, device_configuration);
  SamV71SerialCcsdsInit_rx_handler(self);
  SamV71SerialCcsdsInit_tx_handler(self);
  Escaper_init(&self->m_escaper, self->m_encoded_packet_buffer,
               Serial_CCSDS_SAMV71_ENCODED_PACKET_MAX_SIZE,
               self->m_decoded_packet_buffer,
               Serial_CCSDS_SAMV71_DECODED_PACKET_MAX_SIZE);

  const char *devname = SamV71_device_to_string(device_configuration->devname);

  self->m_task = xTaskCreateStatic(
      SamV71SerialCcsdsPoll, devname, DRIVER_TASK_STACK_SIZE, self,
      DRIVER_TASK_PRIORITY, self->m_task_stack_buffer, &self->m_task_buffer);
}

void SamV71SerialCcsdsPoll(void *private_data) {
  samv71_serial_ccsds_private_data *self =
      (samv71_serial_ccsds_private_data *)private_data;
  size_t length = 0;

  Escaper_start_decoder(&self->m_escaper);
  xSemaphoreTake(self->m_rx_semaphore, portMAX_DELAY);
  Hal_uart_read(&self->m_hal_uart, self->m_fifo_memory_block,
                Serial_CCSDS_SAMV71_RECV_BUFFER_SIZE, self->m_uart_rx_handler);

  while (true) {
    /// Wait for data to arrive. Semaphore will be given
    xSemaphoreTake(self->m_rx_semaphore, portMAX_DELAY);
    length = ByteFifo_getCount(&self->m_hal_uart.rxFifo);
    if (length <= 0) {
      Hal_console_usart_write(
          (const uint8_t *const)SAMV71_SERIAL_CCSDS_POOL_ERROR,
          sizeof(SAMV71_SERIAL_CCSDS_POOL_ERROR));
      assert(false && SAMV71_SERIAL_CCSDS_POOL_ERROR);
      return;
    } else {
      for (size_t i = 0; i < length; i++) {
        ByteFifo_pull(&self->m_hal_uart.rxFifo, &self->m_recv_buffer[i]);
      }
      Escaper_decode_packet(&self->m_escaper, self->m_recv_buffer, length,
                            Broker_receive_packet);
    }
  }
}

void SamV71SerialCcsdsSend(void *private_data, const uint8_t *const data,
                           const size_t length) {
  samv71_serial_ccsds_private_data *self =
      (samv71_serial_ccsds_private_data *)private_data;

  size_t index = 0;
  size_t packetLength = 0;

  Escaper_start_encoder(&self->m_escaper);
  while (index < length) {
    packetLength =
        Escaper_encode_packet(&self->m_escaper, data, length, &index);
    xSemaphoreTake(self->m_tx_semaphore, portMAX_DELAY);
    Hal_uart_write(&self->m_hal_uart,
                   (uint8_t *const) & self->m_encoded_packet_buffer,
                   packetLength, &self->m_uart_tx_handler);
  }
}
