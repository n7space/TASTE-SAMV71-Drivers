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

#include <Hal/Hal.h>
#include <Init/Init.h>

#include <samv71_serial_ccsds/samv71_serial_ccsds.h>

#define NUMBER_OF_DEVICES 1

Hal_Uart halUart;

void vApplicationMallocFailedHook();
void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName);
void vApplicationIdleHook();
void vApplicationTickHook();

void device1_interface_deliver_function(const uint8_t *const data,
                                        const size_t data_size) {
  (void)data;
  (void)data_size;
  // todo implement sending back received data
}

void *bus_to_driver_private_data[NUMBER_OF_DEVICES];
void *bus_to_driver_send_function[NUMBER_OF_DEVICES];
void *interface_to_deliver_function[NUMBER_OF_DEVICES] = {
    (void *)(device1_interface_deliver_function)};

int main(void) {
  Init_setup_hardware();

  samv71_serial_ccsds_private_data serial = {.m_device = uart4};
  Serial_CCSDS_SamV71_Conf_T device = {
      .devname = "device",
      .speed = b38400,
      .parity = 0,
      .bits = 8,
      .use_paritybit = 0,
  };
  SamV71SerialCcsdsInit(&serial, BUS_INVALID_ID, DEVICE_INVALID_ID, &device,
                        NULL);
  vTaskStartScheduler();

  return 0;
}

void vApplicationMallocFailedHook() { configASSERT(0); }

void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName) {
  (void)pxTask;
  (void)pcTaskName;

  configASSERT(0);
}

void vApplicationIdleHook() {
  volatile size_t xFreeHeapSpace = xPortGetFreeHeapSize();
  (void)xFreeHeapSpace;
}

void vApplicationTickHook() {}
