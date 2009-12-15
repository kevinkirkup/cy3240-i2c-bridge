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
#include "config.h"
#include "cy3240.h"
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
          const uint8* const pSendData,
          const uint16* const pSendLength,
          uint8* const pReceiveData,
          uint16* const pReceiveLength
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
          error = pCy3240->write(
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
          error = pCy3240->read(
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
 *  Method to pack the reconfigure packet for the bridge controller
 *
 *  @param pCy3240 [in] the CY3240 state
 *  @param pLength [out] the length of the reconfigure data
 *  @returns Cy3240_Error_t
 */
//-----------------------------------------------------------------------------
static Cy3240_Error_t
pack_reconfigure(
          const Cy3240_t* const pCy3240,
          uint16* const pLength
          )
{
     // Check the parameters
     if (pCy3240 != NULL) {

          // Initialize the byte index
          uint8 byteIndex = 0;

          SEND_PACKET[byteIndex++] = CONTROL_BYTE_I2C_WRITE | CONTROL_BYTE_START | pCy3240->clock;
          SEND_PACKET[byteIndex++] = LENGTH_BYTE_LAST_PACKET | 0x01;

          // Set the I2C address of the CY3240 control register
          SEND_PACKET[byteIndex++] = CONTROL_I2C_ADDRESS;

          // Set the power mode to use
          SEND_PACKET[byteIndex] |= pCy3240->power;

          // Set the length
          *pLength = byteIndex + 1;

          return CY3240_ERROR_OK;
     }

     return CY3240_ERROR_INVALID_PARAMETERS;
}   /* -----  end of static function pack_reconfigure  ----- */

//-----------------------------------------------------------------------------
/**
 *  Method to pack the reconfigure packet for the bridge controller
 *
 *  @param pLength [out] the length of the reinit data
 *  @returns Cy3240_Error_t
 */
//-----------------------------------------------------------------------------
static Cy3240_Error_t
pack_reinit(
          uint16* const pLength
          )
{
     if (pLength != NULL) {

          // Initialize the byte index
          uint8 byteIndex = 0;

          SEND_PACKET[byteIndex++] = CONTROL_BYTE_I2C_WRITE | CONTROL_BYTE_REINIT;
          SEND_PACKET[byteIndex++] = LENGTH_BYTE_LAST_PACKET;

          // Set the control address
          SEND_PACKET[byteIndex++] = CONTROL_I2C_ADDRESS;

          // Set the length
          *pLength = byteIndex + 1;

          return CY3240_ERROR_OK;
     }

     return CY3240_ERROR_INVALID_PARAMETERS;
}   /* -----  end of static function pack_reconfigure  ----- */


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
Cy3240_Error_t
pack_write_input(
          uint8 address,
          const uint8* const pSendData,
          uint16* const pSendLength,
          bool first,
          bool more
          )
{
     // Check parameters
     if ((pSendData != NULL) &&
         (pSendLength != NULL) &&
         (*pSendLength != 0)) {

          // Initialize the byte index
          uint8 byteIndex = 0;

          SEND_PACKET[byteIndex++] = CONTROL_BYTE_I2C_WRITE | CONTROL_BYTE_START;
          SEND_PACKET[byteIndex++] = (uint8)*pSendLength;

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
          const uint16* const pWriteLength,
          const uint16* const pReadLength,
          uint16* const pBytesLeft
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
          uint8 address,
          uint16* const pReadLength,
          bool first,
          bool more
          )
{

     // Check the parameters
     if ((pReadLength != NULL) &&
         (*pReadLength != 0)) {

          uint8 byteIndex = 0;

          SEND_PACKET[byteIndex++] = CONTROL_BYTE_I2C_READ | CONTROL_BYTE_START;
          SEND_PACKET[byteIndex++] = (uint8)*pReadLength;

          // If there is more data, set the more bit
          if (more)
               SEND_PACKET[INPUT_PACKET_INDEX_LENGTH] |= LENGTH_BYTE_MORE_PACKETS;

          else
               SEND_PACKET[INPUT_PACKET_INDEX_CMD] |= CONTROL_BYTE_STOP;

          // If this is the first packet, we need to send the address
          if (first)
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
          uint8* const pData,
          const uint16* const pLength
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

//@} End of Private Methods


//////////////////////////////////////////////////////////////////////
/// @name Methods
//@{

//-----------------------------------------------------------------------------
Cy3240_Error_t
cy3240_reconfigure(
          const Cy3240_t* const pCy3240,
          bool restart,
          bool reinit
          )
{

     // Check the parameters
     if (pCy3240 != NULL) {

          Cy3240_Error_t result = CY3240_ERROR_OK;

          uint16 writeLength = 0;
          uint16 readLength = 0;

          // TODO:
          // Need to be able to retransmit data if Nack is received
          if CY3240_SUCCESS(result) {

               // Setup the reinit command
               if (reinit) {

                    result = pack_reinit(
                              &writeLength);

               // Setup the reconfigure command
               } else {

                    result = pack_reconfigure(
                              pCy3240,
                              &writeLength);
               }

               if CY3240_FAILURE(result)
                    printf("\nFailed to pack send data in write input packet: %i", result);
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
                    printf("\nFailed to transmit write packet");
          }

          return result;

     }

     return CY3240_ERROR_INVALID_PARAMETERS;
}


//-----------------------------------------------------------------------------
Cy3240_Error_t
cy3240_write(
          Cy3240_t* const pCy3240,
          uint8 address,
          const uint8* const pData,
          uint16* const pLength
          )
{

     if ((pData != NULL) &&
         (pLength != NULL) &&
         (*pLength != 0)) {

          Cy3240_Error_t result = CY3240_ERROR_OK;

          // TODO: Move the WriteStart point after each write
          uint16 writeLength;
          uint16 readLength;
          uint8* pWriteStart = pData;
          uint16 bytesLeft = *pLength;

          bool first = true;

          while (CY3240_SUCCESS(result) && (bytesLeft > 0)) {

               // Are there going to be more segments
               bool more;

               // Check if there will be more segments
               if (bytesLeft > CY3240_MAX_SIZE_DATA)
                    more = true;
               else
                    more = false;

               // Set the write and read length to tranfer one packet at a time
               writeLength = MIN(bytesLeft, CY3240_MAX_SIZE_DATA);
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
          const Cy3240_t* const pCy3240,
          uint8 address,
          uint8* const pData,
          uint16* const pLength
          )
{

     if ((pCy3240 != NULL) &&
         (pData != NULL) &&
         (pLength != NULL) &&
         (*pLength != 0)) {

          Cy3240_Error_t result = CY3240_ERROR_OK;

          // TODO: Move the ReadStart point after each write
          uint16 readLength;
          uint8* pReadStart = pData;
          const uint16 writeLength = READ_INPUT_PACKET_SIZE;
          uint16 bytesLeft = *pLength;
          bool first = true;

          while (CY3240_SUCCESS(result) && (bytesLeft > 0)) {

               // Are there going to be more segments
               bool more;

               // Check if there will be more segments
               if (bytesLeft > CY3240_MAX_SIZE_DATA)
                    more = true;
               else
                    more = false;

               // Calculate the number of packets to read
               readLength = MIN(bytesLeft, CY3240_MAX_SIZE_DATA);

               // Create the read input packet
               if (CY3240_SUCCESS(result)) {

                    result = pack_read_input(
                              address,
                              &readLength,
                              first,
                              more);

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

               // No longer the first packet
               first = false;
          }

          return result;
     }

     return CY3240_ERROR_INVALID_PARAMETERS;
}

//-----------------------------------------------------------------------------
Cy3240_Error_t
cy3240_open(
          Cy3240_t* const pCy3240
          )
{

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
          error = pCy3240->init();
          if (HID_FAILURE(error)) {
               fprintf(stderr, "hid_init failed with return code %d\n", error);
               return CY3240_ERROR_HID;
          }

          // Create the interface to the device
          pCy3240->pHid = hid_new_HIDInterface();
          if (pCy3240->pHid == 0) {
               fprintf(stderr, "hid_new_HIDInterface() failed, out of memory?\n");
               return CY3240_ERROR_HID;
          }

          // For open the usb device
          error = hid_force_open(
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
          Cy3240_t* const pCy3240
          )
{
     if (pCy3240 != NULL) {

          hid_return error = HID_RET_SUCCESS;

          /* End Cypress Customizations */
          error = hid_close(pCy3240->pHid);
          if (error != HID_RET_SUCCESS) {
            fprintf(stderr, "hid_close failed with return code %d\n", error);
            return CY3240_ERROR_HID;
          }

          hid_delete_HIDInterface(&pCy3240->pHid);

          error = hid_cleanup();
          if (error != HID_RET_SUCCESS) {
            fprintf(stderr, "hid_cleanup failed with return code %d\n", error);
            return CY3240_ERROR_HID;
          }

          return CY3240_ERROR_OK;
     }

     return CY3240_ERROR_INVALID_PARAMETERS;
}

//-----------------------------------------------------------------------------
Cy3240_Error_t
cy3240_util_factory (
          Cy3240_t* pCy3240,
          int iface_number,
          int timeout,
          Cy3240_Power_t power,
          Cy3240_Bus_t bus,
          Cy3240_I2C_ClockSpeed_t clock
          )
{
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
          pCy3240->init = hid_init;
          pCy3240->write = hid_interrupt_write;
          pCy3240->read = hid_interrupt_read;

          return CY3240_ERROR_OK;
     }

     return CY3240_ERROR_INVALID_PARAMETERS;

}   /* -----  end of function cy3240_util_factory  ----- */

//@} End of Methods

