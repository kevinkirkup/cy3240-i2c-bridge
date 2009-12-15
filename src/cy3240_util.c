/**
 * @file cy3240_util.c
 *
 * @brief Utility functions for the CY3240
 *
 * Utility functions for the CY3240
 *
 * @ingroup CY3240
 *
 * @owner  Kevin Kirkup (kevin.kirkup@gmail.com)
 * @author Kevin Kirkup (kevin.kirkup@gmail.com)
 */

//////////////////////////////////////////////////////////////////////
/// @name Includes
//@{

#include <stdio.h>
#include "hid.h"
#include "cy3240_types.h"
#include "cy3240_util.h"

//@} End of Includes


//////////////////////////////////////////////////////////////////////
/// @name Methods
//@{

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

//-----------------------------------------------------------------------------
bool
cy3240_util_match_serial_number(
    struct usb_dev_handle* usbdev,
    void* custom,
    unsigned int len
    )
{
  bool ret;

  // Allocate a buffer to read the current usb device's serial number
  char* buffer = (char*)malloc (len);

  // Get the serial number of the specfied device
  usb_get_string_simple(
            usbdev,
            usb_device(usbdev)->descriptor.iSerialNumber,
            buffer,
            len);

  // Compare the current serial number with the one we are looking for
  ret = strncmp(buffer, (char*)custom, len) == 0;

  // Free the temporary buffer
  free(buffer);

  // Return the result of the compare
  return ret;
}

//@} End of Methods
