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

#define DRIVER_TASK_STACK_SIZE configMINIMAL_STACK_SIZE
#define DRIVER_TASK_PRIORITY configMAX_SYSCALL_INTERRUPT_PRIORITY + 1

static inline void
SamV71SerialCcsdsInit_uart_register(samv71_serial_ccsds_private_data *self) {
  switch (self->m_device) {
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
    assert("Not supported device name");
  }
}
static inline void
SamV71SerialCcsdsInit_uart_data_bits(samv71_serial_ccsds_private_data *self,
                                     Serial_CCSDS_SamV71_Conf_T_bits bits) {
  (void)self;
  if (bits != 8) {
    assert("Not supported number of data bits");
  }
}
static inline void SamV71SerialCcsdsInit_uart_parity(
    samv71_serial_ccsds_private_data *self,
    Serial_CCSDS_SamV71_Conf_T_use_paritybit useParity,
    Serial_CCSDS_SamV71_Parity_T parity) {
  if (useParity) {
    switch (parity) {
    case odd:
      self->m_hal_uart_config.parity = Uart_Parity_Odd;
      break;
    case even:
      self->m_hal_uart_config.parity = Uart_Parity_Even;
      break;
    default:
      assert("Not supported parity");
    }
  } else {
    self->m_hal_uart_config.parity = Uart_Parity_None;
  }
}
static inline void
SamV71SerialCcsdsInit_uart_baudrate(samv71_serial_ccsds_private_data *self,
                                    Serial_CCSDS_SamV71_Baudrate_T speed) {

  switch (speed) {
  case b9600:
    self->m_hal_uart_config.baudrate = 9600;
    break;
  case b19200:
    self->m_hal_uart_config.baudrate = 19200;
    break;
  case b38400:
    self->m_hal_uart_config.baudrate = 38400;
    break;
  case b57600:
    self->m_hal_uart_config.baudrate = 57600;
    break;
  case b115200:
    self->m_hal_uart_config.baudrate = 115200;
    break;
  case b230400:
    self->m_hal_uart_config.baudrate = 230400;
    break;
  default:
    assert("Not supported baudrate");
    break;
  }
}

static inline void SamV71SerialCcsdsInit_uart_init(
    samv71_serial_ccsds_private_data *const self,
    const Serial_CCSDS_SamV71_Conf_T *const device_configuration) {
  SamV71SerialCcsdsInit_uart_register(self);
  SamV71SerialCcsdsInit_uart_data_bits(self, device_configuration->bits);
  SamV71SerialCcsdsInit_uart_parity(self, device_configuration->use_paritybit,
                                    device_configuration->parity);
  SamV71SerialCcsdsInit_uart_baudrate(self, device_configuration->speed);

  Hal_uart_init(&self->m_hal_uart, self->m_hal_uart_config);
}

void UartRxCallback(void *private_data) {
  samv71_serial_ccsds_private_data *self =
      (samv71_serial_ccsds_private_data *)private_data;

  xSemaphoreGiveFromISR(self->m_rx_semaphore, NULL);
}
ByteFifo *UartTxCallback(void *private_data) {
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
  self->m_uart_rx_handler.targetCharacter = ESCAPE_BYTE;
  self->m_uart_rx_handler.targetLength = Serial_CCSDS_SAMV71_RECV_BUFFER_SIZE;
  vSemaphoreCreateBinary(self->m_rx_semaphore);
  xSemaphoreGive(self->m_rx_semaphore);
}

static inline void
SamV71SerialCcsdsInit_tx_handler(samv71_serial_ccsds_private_data *const self) {
  self->m_uart_tx_handler.callback = UartTxCallback;
  self->m_uart_tx_handler.arg = self;
  vSemaphoreCreateBinary(self->m_tx_semaphore);
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
               Serial_CCSDS_SAMV71_ENCODED_PACKET_BUFFER_SIZE,
               self->m_decoded_packet_buffer,
               Serial_CCSDS_SAMV71_DECODED_PACKET_BUFFER_SIZE);

  xTaskCreate(SamV71SerialCcsdsPoll, device_configuration->devname,
              DRIVER_TASK_STACK_SIZE, self, DRIVER_TASK_PRIORITY,
              &self->m_task);
}

void SamV71SerialCcsdsPoll(void *private_data) {
  samv71_serial_ccsds_private_data *self =
      (samv71_serial_ccsds_private_data *)private_data;

  size_t length = 0;

  Escaper_start_decoder(&self->m_escaper);
  Hal_uart_read(&self->m_hal_uart, self->m_recv_buffer,
                Serial_CCSDS_SAMV71_RECV_BUFFER_SIZE, self->m_uart_rx_handler);

  while (1) {
    xSemaphoreTake(self->m_rx_semaphore, portMAX_DELAY);
    length = ByteFifo_getCount(&self->m_hal_uart.rxFifo);
    if (length > 0) {
      for (size_t i = 0; i < length; i++) {
        ByteFifo_pull(&self->m_hal_uart.rxFifo, self->m_recv_buffer);
      }
      Escaper_decode_packet(&self->m_escaper, self->m_recv_buffer, length,
                            Broker_receive_packet);
    } else {
      return;
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
    Hal_uart_write(&self->m_hal_uart, data, packetLength,
                   self->m_uart_tx_handler);
    xSemaphoreTake(self->m_tx_semaphore, portMAX_DELAY);
  }
}
