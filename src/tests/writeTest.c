/**
 * @file writeTest.c
 *
 * @brief Write test for the CY3240 code
 *
 * Write test for the CY3240 code
 *
 * @ingroup Write
 *
 * @owner  Kevin Kirkup (kevin.kirkup@gmail.com)
 * @author Kevin Kirkup (kevin.kirkup@gmail.com)
 */

//////////////////////////////////////////////////////////////////////
/// @name Includes
//@{

#include "config.h"
#include "string.h"
#include "cy3240.h"
#include "cy3240_types.h"
#include "cy3240_util.h"
#include "cy3240_packet.h"
#include "writeTest.h"
#include "AceUnitData.h"

//@} End of Includes


//////////////////////////////////////////////////////////////////////
/// @name Defines
//@{

#define MY_ADDRESS             (0)
#define SEND_BUFFER_SIZE       (128)
#define RECEIVE_BUFFER_SIZE    (128)

#if DEBUG
#define DBG(x) x

#else
#define DBG(x)

#endif // DEBUG

//@} End of Defines


//////////////////////////////////////////////////////////////////////
/// @name Data
//@{

/**
 * The component that will be used for the unit tests.
 */
Cy3240_t myData;

// The sending buffer
uint8 SEND_BUFFER[SEND_BUFFER_SIZE] = {0};
uint8 RECEIVE_BUFFER[SEND_BUFFER_SIZE] = {0};

// The location where data should be written in the send buffer
uint8* pWrite;

//@} End of Data


//////////////////////////////////////////////////////////////////////
/// @name Methods
//@{


//-----------------------------------------------------------------------------
/**
 *  Substitute method for the HID init
 *
 *  @see hid.h
 *  @returns hid_return
 */
//-----------------------------------------------------------------------------
hid_return
init_test(
          void
          )
{
     DBG(printf("HID Init\n");)

     return HID_RET_SUCCESS;
}   /* -----  end of static function init_test  ----- */

//-----------------------------------------------------------------------------
/**
 *  Substitute method for the HID write
 *
 *  @see hid.h
 *  @returns hid_return
 */
//-----------------------------------------------------------------------------
hid_return
write_test(
          HIDInterface* const hidif,
          unsigned int const ep,
          const char* bytes,
          unsigned int const size,
          unsigned int const timeout
          )
{
     DBG(printf("HID Write\n");)

     // Write the data to the send buffer
     memcpy(pWrite, bytes, size);

     // Move the write pointer
     pWrite += size;

     return HID_RET_SUCCESS;
}

//-----------------------------------------------------------------------------
/**
 *  Substitute method for the HID read
 *
 *  @see hid.h
 *  @returns hid_return
 */
//-----------------------------------------------------------------------------
hid_return
read_test(
          HIDInterface* const hidif,
          unsigned int const ep,
          char* const bytes,
          unsigned int const size,
          unsigned int const timeout
          )
{
     DBG(printf("HID Read\n");)

     // Copy the acknowledgments in the return buffer
     memcpy(bytes, RECEIVE_BUFFER, size);

     // Set the first bit to something unique
     bytes[0] = 0x07;

     return HID_RET_SUCCESS;
}

//-----------------------------------------------------------------------------
/**
 *  Setup before each test case
 */
//-----------------------------------------------------------------------------
A_Before void
testSetup (
          void
          )
{

     Cy3240_Error_t result = CY3240_ERROR_OK;

     // Initialize the state
     memset(&myData, 0x00, sizeof(Cy3240_t));

     // Initialize the send buffer
     memset(SEND_BUFFER, 0x00, sizeof(SEND_BUFFER));

     // Initialize the write location
     pWrite = SEND_BUFFER;

     // Fill the receive buffer with ack bytes
     memset(RECEIVE_BUFFER, TX_ACK, sizeof(RECEIVE_BUFFER));

     // Initialize the state
     result = cy3240_util_factory(
               &myData,
               0,
               1000,
               CY3240_POWER_5V,
               CY3240_BUS_I2C,
               CY3240_100kHz);

     assertTrue("The usb device should be successfully created",
               CY3240_SUCCESS(result));

     // Modify the read and write interfaces to point to our functions
     myData.init = init_test;
     myData.write = write_test;
     myData.read = read_test;

}


//-----------------------------------------------------------------------------
/**
 *  Cleanup after each test case
 */
//-----------------------------------------------------------------------------
A_After void
testCleanup (
          void
          )
{
     // Initialize the state
     memset(&myData, 0x00, sizeof(Cy3240_t));
}

//-----------------------------------------------------------------------------
/**
 *  Error Test Case
 */
//-----------------------------------------------------------------------------
A_Test void
testWriteError(
          void
          )
{
     uint8 data[8] = {0};
     uint16 length = 8;
     Cy3240_Error_t result = CY3240_ERROR_OK;

     // NULL Data buffer
     result = cy3240_write(
               &myData,
               MY_ADDRESS,
               NULL,
               &length);

     assertEquals("Pass write with NULL data buffer should indicate invalid parameter",
               CY3240_ERROR_INVALID_PARAMETERS,
               result);

     // NULL length parameter
     cy3240_write(
               &myData,
               MY_ADDRESS,
               data,
               NULL);

     assertEquals("Passing write with NULL length should indicate invalid parameter",
               CY3240_ERROR_INVALID_PARAMETERS,
               result);

     // 0 length
     length = 0;
     cy3240_write(
               &myData,
               MY_ADDRESS,
               data,
               &length);

     assertEquals("Pass zero for the length should indicate invalid parameter",
               CY3240_ERROR_INVALID_PARAMETERS,
               result);
}

//-----------------------------------------------------------------------------
/**
 *  Test Case for writing a small amount of data i.e. < packet size
 */
//-----------------------------------------------------------------------------
A_Test void
testWriteSmall (
          void
          )
{
     uint8 data[8] = {0};
     uint16 length = 8;
     Cy3240_Error_t result = CY3240_ERROR_OK;

     // Fill the data buffer with a test pattern
     memset(data, 0xAC, sizeof(data));

     // Write a small packet to the device
     result = cy3240_write(
               &myData,
               MY_ADDRESS,
               data,
               &length);

     assertTrue("The write should complete successfully",
               CY3240_SUCCESS(result));

     assertEquals("The control byte should show start, stop, write and I2C: 0x0A",
               0x0A,
               SEND_BUFFER[INPUT_PACKET_INDEX_CMD]);

     assertEquals("The length byte should show length 8 and no more packets: 0x08",
               0x08,
               SEND_BUFFER[INPUT_PACKET_INDEX_LENGTH]);

     assertEquals("The data portion of the send buffer should equal the data buffer",
               0,
               memcmp(data, &SEND_BUFFER[WRITE_INPUT_PACKET_INDEX_DATA], length));

}

//-----------------------------------------------------------------------------
/**
 *  Test Case for writing a medium amount of data i.e. = packet size
 */
//-----------------------------------------------------------------------------
A_Test void
testWriteMedium (
          void
          )
{
     uint8 data[61] = {0};
     uint16 length = 61;
     Cy3240_Error_t result = CY3240_ERROR_OK;

     // Fill the data buffer with a test pattern
     memset(data, 0xAC, sizeof(data));

     // Write a small packet to the device
     result = cy3240_write(
               &myData,
               MY_ADDRESS,
               data,
               &length);

     assertTrue("The write should complete successfully",
               CY3240_SUCCESS(result));

     assertEquals("The control byte should show start, stop, write and I2C: 0x0A",
               0x0A,
               SEND_BUFFER[INPUT_PACKET_INDEX_CMD]);

     assertEquals("The length byte should show length 61 and no more packets: 0x3D",
               0x3D,
               SEND_BUFFER[INPUT_PACKET_INDEX_LENGTH]);

     assertEquals("The data portion of the send buffer should equal the data buffer",
               0,
               memcmp(data, &SEND_BUFFER[WRITE_INPUT_PACKET_INDEX_DATA], length));
}

//-----------------------------------------------------------------------------
/**
 *  Test Case for writing a large amount of data i.e. > packet size
 */
//-----------------------------------------------------------------------------
A_Test void
testWriteLarge (
          void
          )
{
     uint8 data[69] = {0};
     uint16 length = 69;
     Cy3240_Error_t result = CY3240_ERROR_OK;

     // Fill the data buffer with a test pattern
     memset(data, 0xAC, sizeof(data));

     // Write a small packet to the device
     result = cy3240_write(
               &myData,
               MY_ADDRESS,
               data,
               &length);

     assertTrue("The write should complete successfully",
               CY3240_SUCCESS(result));

     assertEquals("The control byte should show start, write and I2C: 0x02",
               0x02,
               SEND_BUFFER[INPUT_PACKET_INDEX_CMD]);

     assertEquals("The length byte should show length 61 and more packets: 0xBD",
               0xBD,
               SEND_BUFFER[INPUT_PACKET_INDEX_LENGTH]);

     assertEquals("The data portion of the send buffer should equal the data buffer",
               0,
               memcmp(data, &SEND_BUFFER[WRITE_INPUT_PACKET_INDEX_DATA], CY3240_MAX_SIZE_DATA));

     // Shift the offset of the buffer for the second packet
     assertEquals("The control byte should show start, stop, write and I2C: 0x0A",
               0x0A,
               SEND_BUFFER[CY3240_MAX_SIZE_PACKET + INPUT_PACKET_INDEX_CMD]);

     assertEquals("The length byte should show length 8 and no more packets: 0x08",
               0x08,
               SEND_BUFFER[CY3240_MAX_SIZE_PACKET + INPUT_PACKET_INDEX_LENGTH]);

     assertEquals("The data portion of the send buffer should equal the data buffer",
               0,
               memcmp(data, &SEND_BUFFER[CY3240_MAX_SIZE_PACKET + WRITE_INPUT_PACKET_INDEX_DATA], 8));

}

//-----------------------------------------------------------------------------
/**
 *  Test Case to test Nack during transmit
 */
//-----------------------------------------------------------------------------
A_Test void
testWriteNack (
          void
          )
{
}

//@} End of Methods

