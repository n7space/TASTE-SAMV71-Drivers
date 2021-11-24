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

#define DRIVER_TASK_STACK_SIZE configMINIMAL_STACK_SIZE
#define DRIVER_TASK_PRIORITY configMAX_SYSCALL_INTERRUPT_PRIORITY + 1

static inline void
SamV71SerialCcsdsInit_register(samv71_serial_ccsds_private_data *self) {
  switch (self->dev) {
  case uart0:
    self->halUartConfig.id = Uart_Id_0;
    break;
  case uart1:
    self->halUartConfig.id = Uart_Id_1;
    break;
  case uart2:
    self->halUartConfig.id = Uart_Id_2;
    break;
  case uart3:
    self->halUartConfig.id = Uart_Id_3;
    break;
  case uart4:
    self->halUartConfig.id = Uart_Id_4;
    break;
  default:
    assert("Not supported device name");
  }
}
static inline void
SamV71SerialCcsdsInit_data_bits(samv71_serial_ccsds_private_data *self,
                                Serial_CCSDS_SamV71_Conf_T_bits bits) {
  if (bits != 8) {
    assert("Not supported number of data bits");
  }
}
static inline void
SamV71SerialCcsdsInit_parity(samv71_serial_ccsds_private_data *self,
                             Serial_CCSDS_SamV71_Conf_T_use_paritybit useParity,
                             Serial_CCSDS_SamV71_Parity_T parity) {
  if (useParity) {
    switch (parity) {
    case odd:
      self->halUartConfig.parity = Uart_Parity_Odd;
      break;
    case even:
      self->halUartConfig.parity = Uart_Parity_Even;
      break;
    default:
      assert("Not supported parity");
    }
  } else {
    self->halUartConfig.parity = Uart_Parity_None;
  }
}
static inline void
SamV71SerialCcsdsInit_baudrate(samv71_serial_ccsds_private_data *self,
                               Serial_CCSDS_SamV71_Baudrate_T speed) {

  switch (speed) {
  case b9600:
    self->halUartConfig.baudrate = 9600;
    break;
  case b19200:
    self->halUartConfig.baudrate = 19200;
    break;
  case b38400:
    self->halUartConfig.baudrate = 38400;
    break;
  case b57600:
    self->halUartConfig.baudrate = 57600;
    break;
  case b115200:
    self->halUartConfig.baudrate = 115200;
    break;
  case b230400:
    self->halUartConfig.baudrate = 230400;
    break;
  default:
    assert("Not supported baudrate");
    break;
  }
}

void SamV71SerialCcsdsInit(
    void *private_data, const enum SystemBus bus_id,
    const enum SystemDevice device_id,
    const Serial_CCSDS_SamV71_Conf_T *const device_configuration,
    const Serial_CCSDS_SamV71_Conf_T *const remote_device_configuration) {

  samv71_serial_ccsds_private_data *serial =
      (samv71_serial_ccsds_private_data *)private_data;

  SamV71SerialCcsdsInit_register(serial);
  SamV71SerialCcsdsInit_data_bits(serial, device_configuration->bits);
  SamV71SerialCcsdsInit_parity(serial, device_configuration->use_paritybit,
                               device_configuration->parity);
  SamV71SerialCcsdsInit_baudrate(serial, device_configuration->speed);
  Hal_uart_init(&serial->halUart, serial->halUartConfig);

  Escaper_init(&serial->escaper, serial->m_encoded_packet_buffer,
               Serial_CCSDS_SAMV71_ENCODED_PACKET_BUFFER_SIZE,
               serial->m_decoded_packet_buffer,
               Serial_CCSDS_SAMV71_DECODED_PACKET_BUFFER_SIZE);

  // xTaskCreate(SamV71SerialCcsdsPoll, device_configuration->devname,
  //   DRIVER_TASK_STACK_SIZE, serial, DRIVER_TASK_PRIORITY,
  //   serial->m_task);
}

void SamV71SerialCcsdsPoll(void *private_data) {
  samv71_serial_ccsds_private_data *serial =
      (samv71_serial_ccsds_private_data *)private_data;

  // Hal_uart_read()
}

void SamV71SerialCcsdsSend(void *private_data, const uint8_t *const data,
                           const size_t length) {
  samv71_serial_ccsds_private_data *serial =
      (samv71_serial_ccsds_private_data *)private_data;
}
