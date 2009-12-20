/**
 * @file test_cypress.c
 *
 * @brief The Cypress CY3240 USB-I2C Bridge
 *
 * The Cypress CY3240 USB-I2C Bridge
 *
 * @ingroup USB-I2C
 *
 * @owner  Kevin Kirkup (kevin.kirkup@gmail.com)
 * @author Kevin Kirkup (kevin.kirkup@gmail.com)
 */

/* COPYRIGHT --
 *
 * This file is part of libhid, a user-space HID access library.
 * libhid is (c) 2003-2005
 *   Martin F. Krafft <libhid@pobox.madduck.net>
 *   Charles Lepple <clepple@ghz.cc>
 *   Arnaud Quette <arnaud.quette@free.fr> && <arnaud.quette@mgeups.com>
 * and distributed under the terms of the GNU General Public License.
 * See the file ./COPYING in the source distribution for more information.
 *
 * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES
 * OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */


//////////////////////////////////////////////////////////////////////
/// @name Includes
//@{

#include <hid.h>
#include <string.h>
#include <stdint.h>
#include "config.h"
#include "cy3240.h"
#include "cy3240_types.h"
#include "cy3240_private_types.h"
#include "cy3240_debug.h"
#include "cy3240_packet.h"

//@} End of Includes


//////////////////////////////////////////////////////////////////////
/// @name Defines
//@{

#define SEND_PACKET_LEN (64)
#define RECV_PACKET_LEN (64)

/* Result Macros */
#define HID_SUCCESS(s)  ((s == HID_RET_SUCCESS) ? TRUE : FALSE)
#define HID_FAILURE(s)  ((s != HID_RET_SUCCESS) ? TRUE : FALSE)

//@} End of Defines

//////////////////////////////////////////////////////////////////////
/// @name Data
//@{

/**
 * Interrupt endpoints, the primary mechanism to send and receive I2C data
 */
const int INPUT_ENDPOINT   = 0x82;              ///< The input usb endpoint
const int OUTPUT_ENDPOINT  = 0x01;              ///< The output usb endpoint

// The sending buffer
char SEND_PACKET[SEND_PACKET_LEN] = {
     0x03, 0x02, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

// The receive buffer
char RECV_PACKET[RECV_PACKET_LEN] = {
     0x03, 0x02, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

//@} End of Data


//////////////////////////////////////////////////////////////////////
/// @name Private Methods
//@{


#if DEBUG
//-----------------------------------------------------------------------------
/**
 *  Method to enable HID debug messages
 *
 *  @returns hid_return
 */
//-----------------------------------------------------------------------------
static hid_return
enable_hid_debug(
          void
          )
{
     // see include/debug.h for possible values
     hid_set_debug(HID_DEBUG_ALL);
     hid_set_debug_stream(stderr);

     //passed directly to libusb
     hid_set_usb_debug(0);

     return HID_RET_SUCCESS;
}   /* -----  end of static function enable_debug  ----- */

//-----------------------------------------------------------------------------
/**
 *  Method to disable the USB HID debugging
 *
 *  @returns hid_return
 */
//-----------------------------------------------------------------------------
static hid_return
disable_hid_debug(
          void
          )
{
     // Configure hid debugging
     // see include/debug.h for possible values
     hid_set_debug(HID_DEBUG_NONE);
     hid_set_debug_stream(stderr);
     hid_set_usb_debug(0);

     return HID_RET_SUCCESS;
}
#endif // DEBUG


//-----------------------------------------------------------------------------
/**
 *  Method to Transmit and Receive a packet from the CY3240
 *
 *  @param pCy3240        [in] the Cypress 3240 status structure
 *  @param pSendData      [in] the data to send
 *  @param pSendLength    [in] the length of the send data
 *  @param pReceiveData   [out] the data received from the Cypress 3240
 *  @param pReceiveLength [out] the length of the received data
 *  @returns Cy3240_Error_t
 */
//-----------------------------------------------------------------------------
static Cy3240_Error_t
transcieve(
          const Cy3240_t* const pCy3240,
          const uint8_t* const pSendData,
          const uint16_t* const pSendLength,
          uint8_t* const pReceiveData,
          uint16_t* const pReceiveLength
          )
{
     if ((pCy3240 != NULL) &&
         (pSendData != NULL) &&
         (pSendLength != NULL) &&
         (*pSendLength != 0) &&
         (pReceiveData != NULL) &&
         (pReceiveLength != NULL) &&
         (*pReceiveLength != 0)) {

          hid_return error = HID_RET_SUCCESS;

          CY3240_DEBUG_PRINT_TX_PACKET(pSendData, *pSendLength);

          // Write the data to the USB HID device
          error = pCy3240->w.write(
                    pCy3240->pHid,
                    OUTPUT_ENDPOINT,
                    pSendData,
                    SEND_PACKET_LEN,
                    pCy3240->timeout);

          if (error != HID_RET_SUCCESS) {
            fprintf(stderr, "hid_set_output_report failed with return code %d\n", error);
            return CY3240_ERROR_HID;
          }

          // Read the response data from the USB HID device
          error = pCy3240->w.read(
                    pCy3240->pHid,
                    INPUT_ENDPOINT,
                    pReceiveData,
                    RECV_PACKET_LEN,
                    pCy3240->timeout);

          if (error != HID_RET_SUCCESS) {
            fprintf(stderr, "hid_get_input_report failed with return code %d\n", error);
            return CY3240_ERROR_HID;
          }

          CY3240_DEBUG_PRINT_RX_PACKET(pReceiveData, *pReceiveLength);

          return CY3240_ERROR_OK;
     }
}


//-----------------------------------------------------------------------------
/**
 *  Method to pack the packet to change the power mode for the bridge controller
 *
 *  @param pCy3240 [in] the CY3240 state
 *  @param pLength [out] the length of the reconfigure data
 *  @returns Cy3240_Error_t
 */
//-----------------------------------------------------------------------------
static Cy3240_Error_t
pack_reconfigure_power(
          Cy3240_Power_t power,
          uint16_t* const pLength
          )
{
     // Check the parameters
     if (pLength != NULL) {

          // Initialize the byte index
          uint8_t byteIndex = 0;

          SEND_PACKET[byteIndex++] = CONTROL_BYTE_I2C_WRITE | CONTROL_BYTE_START;
          SEND_PACKET[byteIndex++] = LENGTH_BYTE_LAST_PACKET | 0x01;

          // Set the I2C address of the CY3240 control register
          SEND_PACKET[byteIndex++] = CONTROL_I2C_ADDRESS;

          // Set the power mode to use
          SEND_PACKET[byteIndex] = power;

          // Set the length
          *pLength = byteIndex + 1;

          return CY3240_ERROR_OK;
     }

     return CY3240_ERROR_INVALID_PARAMETERS;
}   /* -----  end of static function pack_reconfigure_power  ----- */

//-----------------------------------------------------------------------------
/**
 *  Method to pack the reconfigure the clock speed for the bridge controller
 *
 *  @param pCy3240 [in] the CY3240 state
 *  @param pLength [out] the length of the reconfigure data
 *  @returns Cy3240_Error_t
 */
//-----------------------------------------------------------------------------
static Cy3240_Error_t
pack_reconfigure_clock(
          Cy3240_I2C_ClockSpeed_t clock,
          uint16_t* const pLength
          )
{
     // Check the parameters
     if (pLength != NULL) {

          // Initialize the byte index
          uint8_t byteIndex = 0;

          SEND_PACKET[byteIndex++] = CONTROL_BYTE_RECONFIG | clock;
          SEND_PACKET[byteIndex++] = LENGTH_BYTE_LAST_PACKET;

          // Set the I2C address of the CY3240 control register
          SEND_PACKET[byteIndex++] = CONTROL_I2C_ADDRESS;

          // Set the length
          *pLength = byteIndex + 1;

          return CY3240_ERROR_OK;
     }

     return CY3240_ERROR_INVALID_PARAMETERS;
}   /* -----  end of static function pack_reconfigure_clock ----- */


//-----------------------------------------------------------------------------
/**
 *  Method to pack the restart the bridge controller
 *
 *  @param pLength [out] the length of the restart data
 *  @returns Cy3240_Error_t
 */
//-----------------------------------------------------------------------------
static Cy3240_Error_t
pack_restart(
          uint16_t* const pLength
          )
{
     if (pLength != NULL) {

          // Initialize the byte index
          uint8_t byteIndex = 0;

          SEND_PACKET[byteIndex++] = CONTROL_BYTE_I2C_WRITE | CONTROL_BYTE_RESTART;
          SEND_PACKET[byteIndex++] = LENGTH_BYTE_LAST_PACKET;

          // Set the control address
          SEND_PACKET[byteIndex++] = CONTROL_I2C_ADDRESS;

          // Set the length
          *pLength = byteIndex + 1;

          return CY3240_ERROR_OK;
     }

     return CY3240_ERROR_INVALID_PARAMETERS;
}   /* -----  end of static function pack_restart  ----- */

//-----------------------------------------------------------------------------
/**
 *  Method to pack the re-initialize the bridge controller
 *
 *  @param pLength [out] the length of the reinit data
 *  @returns Cy3240_Error_t
 */
//-----------------------------------------------------------------------------
static Cy3240_Error_t
pack_reinit(
          uint16_t* const pLength
          )
{
     if (pLength != NULL) {

          // Initialize the byte index
          uint8_t byteIndex = 0;

          SEND_PACKET[byteIndex++] = CONTROL_BYTE_I2C_WRITE | CONTROL_BYTE_REINIT;
          SEND_PACKET[byteIndex++] = LENGTH_BYTE_LAST_PACKET;

          // Set the control address
          SEND_PACKET[byteIndex++] = CONTROL_I2C_ADDRESS;

          // Set the length
          *pLength = byteIndex + 1;

          return CY3240_ERROR_OK;
     }

     return CY3240_ERROR_INVALID_PARAMETERS;
}   /* -----  end of static function pack_reinit  ----- */


//-----------------------------------------------------------------------------
/**
 *  Method to pack a data write input packet
 *
 *  @param address     [in] the I2C address of the target
 *  @param pSendData   [in] the data to send
 *  @param pSendLength [in] the length of the data to send
 *  @param first       [in] is this the first packet
 *  @param more        [in] are there more packets
 *  @returns Cy3240_Error_t
 */
//-----------------------------------------------------------------------------
static Cy3240_Error_t
pack_write_input(
          uint8_t address,
          const uint8_t* const pSendData,
          uint16_t* const pSendLength,
          bool first,
          bool more
          )
{
     // Check parameters
     if ((pSendData != NULL) &&
         (pSendLength != NULL) &&
         (*pSendLength != 0)) {

          // Initialize the byte index
          uint8_t byteIndex = 0;

          SEND_PACKET[byteIndex++] = CONTROL_BYTE_I2C_WRITE | CONTROL_BYTE_START;
          SEND_PACKET[byteIndex++] = (uint8_t)*pSendLength;

          // Check to see if this is the last packet
          if (more)
               SEND_PACKET[INPUT_PACKET_INDEX_LENGTH] |= LENGTH_BYTE_MORE_PACKETS;

          else
               SEND_PACKET[INPUT_PACKET_INDEX_CMD] |= CONTROL_BYTE_STOP;

          // If this is the first packet, we need to send the address
          if (first)
               SEND_PACKET[byteIndex++] = address;

          // Copy the data in to the send buffer
          memcpy(&SEND_PACKET[byteIndex], pSendData, *pSendLength);

          // Update the length to include the header bytes
          *pSendLength += byteIndex;

          return CY3240_ERROR_OK;
     }

     return CY3240_ERROR_INVALID_PARAMETERS;
}   /* -----  end of static function pack_write  ----- */

//-----------------------------------------------------------------------------
/**
 *  Method to decode the write output packet
 *
 *  @param pWriteLength [in] the number of bytes written
 *  @param pReadLength  [in] the number of bytes read
 *  @param pBytesLeft   [out] the number of bytes left to send
 *  @returns Cy3240_Error_t
 */
//-----------------------------------------------------------------------------
static Cy3240_Error_t
unpack_write_output(
          const uint16_t* const pWriteLength,
          const uint16_t* const pReadLength,
          uint16_t* const pBytesLeft
          )
{
     // Check the parameters
     if ((pWriteLength != NULL) &&
         (*pWriteLength != 0) &&
         (pReadLength != NULL) &&
         (*pReadLength != 0) &&
         (RECV_PACKET[OUTPUT_PACKET_INDEX_STATUS] != 0x00)) {

          int x = 0;

          // Loop through the pack acknowledgments
          for (x = OUTPUT_PACKET_INDEX_STATUS + 1; x < *pReadLength; x++) {

               // Check for ack
               if (RECV_PACKET[x] == TX_ACK) {

                    // Decrement the number of remaining bytes
                    if (pBytesLeft != NULL) {
                         *pBytesLeft = *pBytesLeft - 1;
                    }

               // Nack
               } else {
                    printf(" Nack: %i", x);
                    return CY3240_ERROR_TX;
               }
          }

          // Finished
          return CY3240_ERROR_OK;
     }

     return CY3240_ERROR_INVALID_PARAMETERS;
}


//-----------------------------------------------------------------------------
/**
 *  Method to pack the read input packet
 *
 *  @param address     [in] the I2C address of the device to read
 *  @param pReadLength [in] the length of bytes to read
 *  @param first       [in] is this the first read
 *  @param more        [in] is there more data to read
 *  @returns Cy3240_Error_t
 */
//-----------------------------------------------------------------------------
static Cy3240_Error_t
pack_read_input (
          uint8_t address,
          uint16_t* const pReadLength
          )
{

     // Check the parameters
     if ((pReadLength != NULL) &&
         (*pReadLength != 0)) {

          uint8_t byteIndex = 0;

          SEND_PACKET[byteIndex++] = CONTROL_BYTE_I2C_READ | CONTROL_BYTE_START | CONTROL_BYTE_STOP;
          SEND_PACKET[byteIndex++] = (uint8_t)*pReadLength;

          // We need to send the address
          SEND_PACKET[byteIndex++] = address;

          return CY3240_ERROR_OK;
     }

     return CY3240_ERROR_INVALID_PARAMETERS;
}   /* -----  end of static function pack_read_packet  ----- */


//-----------------------------------------------------------------------------
/**
 *  Method to unpack the read output packet data
 *
 *  @param pData   [out] the buffer to read the data into
 *  @param pLength [in] the amount of data to read
 *  @returns Cy3240_Error_t
 */
//-----------------------------------------------------------------------------
static Cy3240_Error_t
unpack_read_output (
          uint8_t* const pData,
          const uint16_t* const pLength
          )
{
     // Check the parameters
     if ((pData != NULL) &&
         (pLength != NULL) &&
         (*pLength != 0) &&
         (RECV_PACKET[OUTPUT_PACKET_INDEX_STATUS] != 0x00)) {

          // Copy the data from the receive buffer
          memcpy(pData, &RECV_PACKET[OUTPUT_PACKET_INDEX_DATA], *pLength);

          return CY3240_ERROR_OK;
     }

     return CY3240_ERROR_INVALID_PARAMETERS;

}   /* -----  end of static function unpack_read_output  ----- */

//-----------------------------------------------------------------------------
/**
 * Method to reconfigure the power mode for the CY3240 bridge chip
 *
 * @param pCy3240 [in] the bridge state inforamtion
 * @param power   [in] the power mode to set
 * @return Cy3240_Error_t
 */
//-----------------------------------------------------------------------------
static Cy3240_Error_t
reconfigure_power(
          Cy3240_t* const pCy3240,
          Cy3240_Power_t power
          )
{
     Cy3240_Error_t result = CY3240_ERROR_OK;
     uint16_t writeLength = 0;
     uint16_t readLength = 0;

     result = pack_reconfigure_power(
               power,
               &writeLength);

     if CY3240_FAILURE(result)
          printf("\nFailed to pack send data in write input packet: %i", result);

     if (CY3240_SUCCESS(result)) {

          readLength = writeLength + CY3240_STATUS_CODE_SIZE;

          // Write the data to the buffer
          // Note! The received data is ignored
          result = transcieve(
                    pCy3240,
                    SEND_PACKET,
                    &writeLength,
                    RECV_PACKET,
                    &readLength);

          if CY3240_FAILURE(result)
               printf("\nFailed to transmit write packet");
     }

     // Set the power mode
     if CY3240_SUCCESS(result)
          pCy3240->power = power;

     return result;
}

//-----------------------------------------------------------------------------
/**
 * Method to set the clock rate for the CY3240 bridge chip
 *
 * @param pCy3240 [in] the bridge state inforamtion
 * @param clock   [in] the clock rate to set
 * @return Cy3240_Error_t
 */
//-----------------------------------------------------------------------------
static Cy3240_Error_t
reconfigure_clock(
          Cy3240_t* const pCy3240,
          Cy3240_I2C_ClockSpeed_t clock
          )
{
     Cy3240_Error_t result = CY3240_ERROR_OK;
     uint16_t writeLength = 0;
     uint16_t readLength = 0;

     result = pack_reconfigure_clock(
               clock,
               &writeLength);

     if CY3240_FAILURE(result)
          printf("\nFailed to pack send data in write input packet: %i", result);

     if (CY3240_SUCCESS(result)) {

          readLength = writeLength + CY3240_STATUS_CODE_SIZE;

          // Write the data to the buffer
          // Note! The received data is ignored
          result = transcieve(
                    pCy3240,
                    SEND_PACKET,
                    &writeLength,
                    RECV_PACKET,
                    &readLength);

          if CY3240_FAILURE(result)
               printf("\nFailed to transmit packet");
     }

     // Set the clock rate
     if CY3240_SUCCESS(result)
          pCy3240->clock = clock;

     return result;
}

//@} End of Private Methods


//////////////////////////////////////////////////////////////////////
/// @name Methods
//@{

//-----------------------------------------------------------------------------
Cy3240_Error_t
cy3240_restart(
          int handle
          )
{
     // The handle is the pointer to the state structure
     const Cy3240_t* pCy3240 = (const Cy3240_t*)handle;

     // Check the parameters
     if (pCy3240 != NULL) {

          Cy3240_Error_t result = CY3240_ERROR_OK;

          uint16_t writeLength = 0;
          uint16_t readLength = 0;

          // TODO: Check the state machine
          if CY3240_SUCCESS(result) {

               result = pack_restart(
                         &writeLength);

               if CY3240_FAILURE(result)
                    printf("\nFailed to pack restart data in write input packet: %i", result);
          }

          if (CY3240_SUCCESS(result)) {

               readLength = writeLength + CY3240_STATUS_CODE_SIZE;

               // Write the data to the buffer
               // Note! The received data is ignored
               result = transcieve(
                         pCy3240,
                         SEND_PACKET,
                         &writeLength,
                         RECV_PACKET,
                         &readLength);

               if CY3240_FAILURE(result)
                    printf("\nFailed to transmit restart packet");
          }

          if (CY3240_SUCCESS(result)) {

               // Decode the response
               result = unpack_write_output(
                         &writeLength,
                         &readLength,
                         NULL);

               if CY3240_FAILURE(result)
                    printf("\nSlave failed to Ack restart");
          }

          return result;

     }

     return CY3240_ERROR_INVALID_PARAMETERS;
}

//-----------------------------------------------------------------------------
Cy3240_Error_t
cy3240_reinit(
          int handle
          )
{
     // The handle is the pointer to the state structure
     const Cy3240_t* pCy3240 = (const Cy3240_t*)handle;

     // Check the parameters
     if (pCy3240 != NULL) {

          Cy3240_Error_t result = CY3240_ERROR_OK;

          uint16_t writeLength = 0;
          uint16_t readLength = 0;

          if CY3240_SUCCESS(result) {

               result = pack_reinit(
                         &writeLength);

               if CY3240_FAILURE(result)
                    printf("\nFailed to pack reinit data in write input packet: %i", result);
          }

          if (CY3240_SUCCESS(result)) {

               readLength = writeLength + CY3240_STATUS_CODE_SIZE;

               // Write the data to the buffer
               // Note! The received data is ignored
               result = transcieve(
                         pCy3240,
                         SEND_PACKET,
                         &writeLength,
                         RECV_PACKET,
                         &readLength);

               if CY3240_FAILURE(result)
                    printf("\nFailed to transmit reinit packet");
          }

          return result;

     }

     return CY3240_ERROR_INVALID_PARAMETERS;
}

//-----------------------------------------------------------------------------
Cy3240_Error_t
cy3240_reconfigure(
          int handle,
          Cy3240_Power_t power,
          Cy3240_Bus_t bus,
          Cy3240_I2C_ClockSpeed_t clock
          )
{
     // The handle is the pointer to the state structure
     Cy3240_t* const pCy3240 = (Cy3240_t* const)handle;

     // Check the parameters
     if (pCy3240 != NULL) {

          Cy3240_Error_t result = CY3240_ERROR_OK;

          // Change the power mode
          if CY3240_SUCCESS(result) {

               result = reconfigure_power(
                         pCy3240,
                         power);

               if CY3240_FAILURE(result)
                    printf("\nFailed to set the requested power mode: %02x", power);
          }

          // Set the clock mode
          if CY3240_SUCCESS(result) {

               result = reconfigure_clock(
                         pCy3240,
                         clock);

               if CY3240_FAILURE(result)
                    printf("\nFailed to set the requested clock mode: %02x", clock);

          }

          // TODO: Changing bus not supported
          pCy3240->bus = bus;

          return result;

     }

     return CY3240_ERROR_INVALID_PARAMETERS;
}


//-----------------------------------------------------------------------------
Cy3240_Error_t
cy3240_write(
          int handle,
          uint8_t address,
          const uint8_t* const pData,
          uint16_t* const pLength
          )
{

     // The handle is the pointer to the state structure
     Cy3240_t* pCy3240 = (Cy3240_t*)handle;

     if ((pCy3240 != NULL) &&
         (pData != NULL) &&
         (pLength != NULL) &&
         (*pLength != 0)) {

          Cy3240_Error_t result = CY3240_ERROR_OK;

          // TODO: Move the WriteStart point after each write
          uint16_t writeLength;
          uint16_t readLength;
          uint8_t* pWriteStart = pData;
          uint16_t bytesLeft = *pLength;

          bool first = true;

          while (CY3240_SUCCESS(result) && (bytesLeft > 0)) {

               // Are there going to be more segments
               bool more;

               // Check if there will be more segments
               if (bytesLeft > CY3240_MAX_WRITE_BYTES)
                    more = true;
               else
                    more = false;

               // Set the write and read length to tranfer one packet at a time
               writeLength = MIN(bytesLeft, CY3240_MAX_WRITE_BYTES);
               readLength = writeLength + CY3240_STATUS_CODE_SIZE;

               // TODO:
               // Need to be able to retransmit data if Nack is received
               if CY3240_SUCCESS(result) {

                    // Pack the data in to the send packet
                    result = pack_write_input(
                              address,
                              pWriteStart,
                              &writeLength,
                              first,
                              more);

                    if CY3240_FAILURE(result)
                         printf("\nFailed to pack send data in write input packet: %i", result);
               }

               if (CY3240_SUCCESS(result)) {

                    // Write the data to the buffer
                    result = transcieve(
                              pCy3240,
                              SEND_PACKET,
                              &writeLength,
                              RECV_PACKET,
                              &readLength);

                    if CY3240_FAILURE(result)
                         printf("\nFailed to transmit write packet");
               }

               if (CY3240_SUCCESS(result)) {

                    // Decode the response
                    result = unpack_write_output(
                              &writeLength,
                              &readLength,
                              &bytesLeft);

                    if CY3240_FAILURE(result)
                         printf("\nFailed to transmit all data");
               }

               // No longer the first time
               first = false;
          }

          return result;
     }

     return CY3240_ERROR_INVALID_PARAMETERS;
}

//-----------------------------------------------------------------------------
Cy3240_Error_t
cy3240_read(
          int handle,
          uint8_t address,
          uint8_t* const pData,
          uint16_t* const pLength
          )
{

     // The handle is the pointer to the state structure
     const Cy3240_t* pCy3240 = (const Cy3240_t*)handle;

     if ((pCy3240 != NULL) &&
         (pData != NULL) &&
         (pLength != NULL) &&
         (*pLength != 0)) {

          Cy3240_Error_t result = CY3240_ERROR_OK;

          uint16_t readLength;
          uint8_t* pReadStart = pData;
          const uint16_t writeLength = READ_INPUT_PACKET_SIZE;
          uint16_t bytesLeft = *pLength;

          // Loop while there is still data to read
          while (CY3240_SUCCESS(result) && (bytesLeft > 0)) {

               // Calculate the number of packets to read
               readLength = MIN(bytesLeft, CY3240_MAX_READ_BYTES);

               // Create the read input packet
               if (CY3240_SUCCESS(result)) {

                    result = pack_read_input(
                              address,
                              &readLength);

                    if CY3240_FAILURE(result)
                         printf("\nFailed to pack read input packet: %i", result);
               }

               // Send the data
               if (CY3240_SUCCESS(result)) {

                    // Write the data to the buffer
                    result = transcieve(
                              pCy3240,
                              SEND_PACKET,
                              &writeLength,
                              RECV_PACKET,
                              &readLength);

                    if CY3240_FAILURE(result)
                         printf("\nFailed to transmit read packet");
               }

               // unpack the result
               if (CY3240_SUCCESS(result)) {

                    // Decode the response
                    result = unpack_read_output(
                              pReadStart,
                              &readLength);

                    if CY3240_FAILURE(result) {
                         printf("\nFailed to read data");

                    } else {
                         pReadStart += readLength;
                         bytesLeft -= readLength;
                    }
               }
          }

          return result;
     }

     return CY3240_ERROR_INVALID_PARAMETERS;
}

//-----------------------------------------------------------------------------
Cy3240_Error_t
cy3240_open(
          int handle
          )
{

     // The handle is the pointer to the state structure
     Cy3240_t* pCy3240 = (Cy3240_t*)handle;

     if (pCy3240 != NULL) {

          hid_return error = HID_RET_SUCCESS;

          HIDInterfaceMatcher matcher = {pCy3240->vendor_id, pCy3240->product_id, NULL, NULL, 0};

#ifdef DEBUG
          error = enable_hid_debug();
          if (HID_FAILURE(error)) {
               fprintf(stderr, "hid debug enable failed with return code %d\n", error);
               return CY3240_ERROR_HID;
          }
#endif

          // Initialize the device
          error = pCy3240->w.init();
          if (HID_FAILURE(error)) {
               fprintf(stderr, "hid_init failed with return code %d\n", error);
               return CY3240_ERROR_HID;
          }

          // Create the interface to the device
          pCy3240->pHid = pCy3240->w.new_if();
          if (pCy3240->pHid == NULL) {
               fprintf(stderr, "hid_new_HIDInterface() failed, out of memory?\n");
               return CY3240_ERROR_HID;
          }

          // For open the usb device
          error = pCy3240->w.force_open(
                    pCy3240->pHid,
                    pCy3240->iface_number,
                    &matcher,
                    3);
          if (HID_FAILURE(error)) {
               fprintf(stderr, "hid_force_open failed with return code %d\n", error);
               return CY3240_ERROR_HID;
          }

#ifdef DEBUG

          error = hid_write_identification(stdout, pCy3240->pHid);
          if (HID_FAILURE(error)) {
               fprintf(stderr, "hid_write_identification failed with return code %d\n", error);
               return CY3240_ERROR_HID;
          }

          error = hid_dump_tree(stdout, pCy3240->pHid);
          if (HID_FAILURE(error)) {
               fprintf(stderr, "hid_dump_tree failed with return code %d\n", error);
               return CY3240_ERROR_HID;
          }

          // Disable HID debugging
          error = disable_hid_debug();
          if (HID_FAILURE(error)) {
               fprintf(stderr, "hid debug disable failed with return code %d\n", error);
               return CY3240_ERROR_HID;
          }
#endif
          return CY3240_ERROR_OK;
     }

     return CY3240_ERROR_INVALID_PARAMETERS;
}

//-----------------------------------------------------------------------------
Cy3240_Error_t
cy3240_close(
          int handle
          )
{
     // The handle is the pointer to the state structure
     Cy3240_t* pCy3240 = (Cy3240_t*)handle;

     if (pCy3240 != NULL) {

          hid_return error = HID_RET_SUCCESS;

          /* End Cypress Customizations */
          error = pCy3240->w.close(pCy3240->pHid);
          if (error != HID_RET_SUCCESS) {
            fprintf(stderr, "hid_close failed with return code %d\n", error);
            return CY3240_ERROR_HID;
          }

          pCy3240->w.delete_if(&pCy3240->pHid);

          error = pCy3240->w.cleanup();
          if (error != HID_RET_SUCCESS) {
            fprintf(stderr, "hid_cleanup failed with return code %d\n", error);
            return CY3240_ERROR_HID;
          }

          // deallocate the handle
          free(pCy3240);

          return CY3240_ERROR_OK;
     }

     return CY3240_ERROR_INVALID_PARAMETERS;
}

//-----------------------------------------------------------------------------
Cy3240_Error_t
cy3240_util_factory (
          int* pHandle,
          int iface_number,
          int timeout,
          Cy3240_Power_t power,
          Cy3240_Bus_t bus,
          Cy3240_I2C_ClockSpeed_t clock
          )
{
     // The handle is the pointer to the state structure
     Cy3240_t* pCy3240;

     // Allocate the handle
     pCy3240 = (Cy3240_t*)malloc(sizeof(Cy3240_t));

     // Check the parameters
     if (pCy3240 != NULL) {

          // Initialize the Cy3240 data structure
          pCy3240->vendor_id = CY3240_VID;
          pCy3240->product_id = CY3240_PID;
          pCy3240->iface_number = iface_number;
          pCy3240->timeout = timeout;
          pCy3240->power = power;
          pCy3240->bus = bus;
          pCy3240->clock = clock;
          pCy3240->w.init = hid_init;
          pCy3240->w.close = hid_close;
          pCy3240->w.write = hid_interrupt_write;
          pCy3240->w.read = hid_interrupt_read;
          pCy3240->w.cleanup = hid_cleanup;
          pCy3240->w.delete_if = hid_delete_HIDInterface;
          pCy3240->w.force_open = hid_force_open;
          pCy3240->w.new_if = hid_new_HIDInterface;

          // Initialize the handle
          *pHandle = pCy3240;

          return CY3240_ERROR_OK;
     }

     return CY3240_ERROR_INVALID_PARAMETERS;

}   /* -----  end of function cy3240_util_factory  ----- */

//@} End of Methods

