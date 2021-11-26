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

#ifndef PACKET_SENDER_H
#define PACKET_SENDER_H

/**
 * @file     PacketSender.h
 * @brief    Part of samv71_serial_ccsds driver demo.
 */

/**
 * @brief   Thread method. Initializes and sends packages
 *
 * Pointer to this function should be pass as xTaskCreate TaskFunciton_t
 * argument. This function will create a constant packet and then it will be
 * sending it every 250ms
 *
 * @param[in]   arg            Pointer to private data passed to the thread
 * method
 */
void SendPacket(void* arg);

#endif
