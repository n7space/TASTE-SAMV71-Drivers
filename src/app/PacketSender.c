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

#include <PacketSender.h>
#include <Packetizer.h>

#include <string.h>

#include <samv71_serial_ccsds/samv71_serial_ccsds.h>

#define TASK_DELAY (100 / portTICK_PERIOD_MS)
#define TASK_MSG "Hi rPi\n\r"
#define PACKET_SIZE                                                            \
  (sizeof(TASK_MSG) + SPACE_PACKET_PRIMARY_HEADER_SIZE +                       \
   SPACE_PACKET_ERROR_CONTROL_SIZE)

inline static void CreatePacket(Packetizer *packetizer,
                                uint8_t *const packetData) {
  memcpy(&packetData[SPACE_PACKET_PRIMARY_HEADER_SIZE], TASK_MSG,
         sizeof(TASK_MSG));
  Packetizer_init(packetizer);
  Packetizer_packetize(packetizer, Packetizer_PacketType_Telemetry, 0, 0,
                       packetData, SPACE_PACKET_PRIMARY_HEADER_SIZE,
                       sizeof(TASK_MSG));
}

void SendPacket(void *args) {
  TickType_t xNextWakeTime;
  Packetizer packetizer;
  samv71_serial_ccsds_private_data *self =
      (samv71_serial_ccsds_private_data *)args;
  uint8_t packetData[PACKET_SIZE] = {0};

  /* Initialise packet to be sent*/
  CreatePacket(&packetizer, packetData);
  /* Initialise xNextWakeTime - this only needs to be done once. */
  xNextWakeTime = xTaskGetTickCount();
  (void)xNextWakeTime;
  for (;;) {
    vTaskDelay(TASK_DELAY);
    SamV71SerialCcsdsSend(self, packetData, PACKET_SIZE);
  }
}
